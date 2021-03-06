/**
 * This file is part of the CernVM File System.
 */

#include <limits>

#include "../logging.h"

template<class CatalogTraversalT, class HashFilterT>
const unsigned int GarbageCollector<CatalogTraversalT,
                                    HashFilterT>::Configuration::kFullHistory =
  std::numeric_limits<unsigned int>::max();

template<class CatalogTraversalT, class HashFilterT>
const unsigned int GarbageCollector<CatalogTraversalT,
                                    HashFilterT>::Configuration::kNoHistory = 0;

template<class CatalogTraversalT, class HashFilterT>
const time_t GarbageCollector<CatalogTraversalT,
                              HashFilterT>::Configuration::kNoTimestamp = 0;


template <class CatalogTraversalT, class HashFilterT>
swissknife::CatalogTraversalParams
  GarbageCollector<CatalogTraversalT, HashFilterT>::GetTraversalParams(
  const GarbageCollector<CatalogTraversalT, HashFilterT>::Configuration &config)
{
  swissknife::CatalogTraversalParams params;
  params.history             = config.keep_history_depth;
  params.timestamp           = config.keep_history_timestamp;
  params.no_repeat_history   = true;
  params.ignore_load_failure = true;
  params.quiet               = ! config.verbose;
  params.repo_url            = config.repo_url;
  params.repo_name           = config.repo_name;
  params.repo_keys           = config.repo_keys;
  params.tmp_dir             = config.tmp_dir;
  return params;
}


template <class CatalogTraversalT, class HashFilterT>
void GarbageCollector<CatalogTraversalT, HashFilterT>::PreserveDataObjects(
 const GarbageCollector<CatalogTraversalT, HashFilterT>::MyCallbackData &data) {
  ++preserved_catalogs_;

  if (configuration_.verbose) {
    if (data.catalog->IsRoot()) {
      LogCvmfs(kLogGc, kLogStdout, "Preserving Revision %d",
                                   data.catalog->revision());
    }
    PrintCatalogTreeEntry(data.tree_level, data.catalog);
  }

  // the hash of the actual catalog needs to preserved
  hash_filter_.Fill(data.catalog->hash());

  // all the objects referenced from this catalog need to be preserved
  const HashVector &referenced_hashes = data.catalog->GetReferencedObjects();
        typename HashVector::const_iterator i    = referenced_hashes.begin();
  const typename HashVector::const_iterator iend = referenced_hashes.end();
  for (; i != iend; ++i) {
    hash_filter_.Fill(*i);
  }
}


template <class CatalogTraversalT, class HashFilterT>
void GarbageCollector<CatalogTraversalT, HashFilterT>::SweepDataObjects(
 const GarbageCollector<CatalogTraversalT, HashFilterT>::MyCallbackData &data) {
  ++condemned_catalogs_;

  if (configuration_.verbose) {
    if (data.catalog->IsRoot()) {
      LogCvmfs(kLogGc, kLogStdout, "Sweeping Revision %d",
                                   data.catalog->revision());
    }
    PrintCatalogTreeEntry(data.tree_level, data.catalog);
  }

  // all the objects referenced from this catalog need to be checked against the
  // the preserved hashes in the hash_filter_ and possibly deleted
  const HashVector &referenced_hashes = data.catalog->GetReferencedObjects();
        typename HashVector::const_iterator i    = referenced_hashes.begin();
  const typename HashVector::const_iterator iend = referenced_hashes.end();
  for (; i != iend; ++i) {
    CheckAndSweep(*i);
  }

  // the catalog itself is also condemned and needs to be removed
  CheckAndSweep(data.catalog->hash());
}


template <class CatalogTraversalT, class HashFilterT>
void GarbageCollector<CatalogTraversalT, HashFilterT>::CheckAndSweep(
                                                       const shash::Any &hash) {
  if (! hash_filter_.Contains(hash)) {
    Sweep(hash);
  }
}


template <class CatalogTraversalT, class HashFilterT>
void GarbageCollector<CatalogTraversalT, HashFilterT>::Sweep(
                                                       const shash::Any &hash) {
  ++condemned_objects_;

  if (configuration_.verbose) {
    LogCvmfs(kLogGc, kLogStdout, "Sweep: %s", hash.ToString().c_str());
  }

  if (configuration_.dry_run) {
    return;
  }

  configuration_.uploader->Remove(hash);
}


template <class CatalogTraversalT, class HashFilterT>
bool GarbageCollector<CatalogTraversalT, HashFilterT>::VerifyConfiguration() const {
  const Configuration &cfg = configuration_;

  if (cfg.uploader == NULL) {
    return false;
  }

  if (cfg.repo_url.empty()) {
    LogCvmfs(kLogGc, kLogDebug, "No repository url provided");
    return false;
  }

  if (cfg.repo_name.empty()) {
    LogCvmfs(kLogGc, kLogDebug, "No repository name provided");
    return false;
  }

  if (cfg.tmp_dir.empty() || ! DirectoryExists(cfg.tmp_dir)) {
    LogCvmfs(kLogGc, kLogDebug, "Temporary directory '%s' doesn't exist",
                                cfg.tmp_dir.c_str());
    return false;
  }

  return true;
}


template <class CatalogTraversalT, class HashFilterT>
bool GarbageCollector<CatalogTraversalT, HashFilterT>::Collect() {
  bool success = true;

  success = VerifyConfiguration();
  if (! success) {
    LogCvmfs(kLogGc, kLogStderr, "Malformed GarbageCollector configuration.");
  }

  success = (success && AnalyzePreservedCatalogTree());
  success = (success && SweepCondemnedCatalogTree());

  return success;
}


template <class CatalogTraversalT, class HashFilterT>
bool GarbageCollector<CatalogTraversalT, HashFilterT>::AnalyzePreservedCatalogTree() {
  if (configuration_.verbose) {
    LogCvmfs(kLogGc, kLogStdout, "Preserving data objects in catalog tree");
  }
  bool success = true;

  typename CatalogTraversalT::callback_t *callback =
    traversal_.RegisterListener(
       &GarbageCollector<CatalogTraversalT, HashFilterT>::PreserveDataObjects,
        this);


  success = traversal_.Traverse() && // traverses the current HEAD
            traversal_.TraverseNamedSnapshots();

  traversal_.UnregisterListener(callback);

  if (success && preserved_catalog_count() == 0) {
    if (configuration_.verbose) {
      LogCvmfs(kLogGc, kLogStderr, "This would delete everything! Abort.");
    }
    success = false;
  }

  return success;
}


template <class CatalogTraversalT, class HashFilterT>
bool GarbageCollector<CatalogTraversalT, HashFilterT>::SweepCondemnedCatalogTree() {
  const bool has_condemned_revisions = (traversal_.pruned_revision_count() > 0);
  if (configuration_.verbose) {
    if (! has_condemned_revisions) {
      LogCvmfs(kLogGc, kLogStdout, "Nothing to be sweeped.");

    } else {
      LogCvmfs(kLogGc, kLogStdout, "Sweeping unreferenced data objects in "
                                   "remaining catalogs");
    }
  }

  if (! has_condemned_revisions) {
    return true;
  }

  typename CatalogTraversalT::callback_t *callback =
    traversal_.RegisterListener(
       &GarbageCollector<CatalogTraversalT, HashFilterT>::SweepDataObjects,
        this);
  const bool success =
             traversal_.TraversePruned(CatalogTraversalT::kDepthFirstTraversal);
  traversal_.UnregisterListener(callback);

  return success;
}


template <class CatalogTraversalT, class HashFilterT>
void GarbageCollector<CatalogTraversalT, HashFilterT>::PrintCatalogTreeEntry(
                                              const unsigned int  tree_level,
                                              const MyCatalog    *catalog) const
{
  std::string tree_indent;
  for (unsigned int i = 0; i < tree_level; ++i) {
    tree_indent += "\u2502  ";
  }
  tree_indent += "\u251C\u2500 ";

  const std::string hash_string = catalog->hash().ToString();
  const std::string path        = (catalog->path().IsEmpty())
                                          ? "/"
                                          : catalog->path().ToString();

  LogCvmfs(kLogGc, kLogStdout, "%s%s %s",
    tree_indent.c_str(),
    hash_string.c_str(),
    path.c_str());
}
