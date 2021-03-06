#
# Define the sources needed to compile the different
# executables in this directory
#

set (CVMFS_LOADER_SOURCES
  smalloc.h atomic.h shortstring.h fs_traversal.h
  platform.h platform_linux.h platform_osx.h
  logging_internal.h logging.h logging.cc
  prng.h
  hash.h hash.cc
  util.h util.cc
  sanitizer.h sanitizer.cc
  options.h options.cc
  globals.h globals.cc
  loader_talk.h loader_talk.cc
  loader.h loader.cc
)

set (CVMFS_CLIENT_SOURCES
  smalloc.h
  logging.cc logging.h logging_internal.h
  tracer.h tracer.cc atomic.h
  duplex_sqlite3.h duplex_curl.h duplex_cares.h
  signature.h signature.cc
  quota.h quota.cc
  hash.h hash.cc
  cache.h cache.cc
  platform.h platform_osx.h platform_linux.h
  monitor.h monitor.cc
  prng.h util.cc util.h
  uuid.h uuid.cc
  backoff.h backoff.cc
  duplex_zlib.h compression.h compression.cc
  sanitizer.cc sanitizer.h
  dns.h dns.cc
  download.cc download.h
  wpad.cc wpad.h
  manifest.h manifest.cc
  whitelist.h whitelist.cc
  manifest_fetch.h manifest_fetch.cc
  murmur.h smallhash.h
  bigvector.h
  lru.h
  globals.h globals.cc
  sql.h sql_impl.h sql.cc
  catalog_sql.h catalog_sql.cc
  catalog.h catalog.cc
  catalog_mgr.h catalog_mgr.cc
  catalog_counters.h catalog_counters_impl.h catalog_counters.cc
  directory_entry.h directory_entry.cc
  shortstring.h
  file_chunk.h file_chunk.cc
  fs_traversal.h
)


set (CVMFS2_DEBUG_SOURCES
  ${CVMFS_CLIENT_SOURCES}

  options.cc options.h
  talk.h talk.cc
  nfs_maps.h nfs_maps.cc
  nfs_shared_maps.h nfs_shared_maps.cc
  glue_buffer.h glue_buffer.cc
  loader.h compat.cc compat.h
  history.h
  history_sql.h history_sql.cc
  history_sqlite.h history_sqlite.cc
  quota_listener.h quota_listener.cc
  auto_umount.h auto_umount.cc
  cvmfs.h cvmfs.cc
)

set (CVMFS2_SOURCES
  ${CVMFS2_DEBUG_SOURCES}
)

set (LIBCVMFS_SOURCES
  ${CVMFS_CLIENT_SOURCES}
  libcvmfs_int.h libcvmfs_int.cc
  libcvmfs.h libcvmfs.cc
)

set (TEST_LIBCVMFS_SOURCES
  libcvmfs.h
  test_libcvmfs.cc
)

set (CVMFS_FSCK_SOURCES
  platform.h platform_linux.h platform_osx.h
  logging_internal.h logging.h logging.cc
  smalloc.h
  atomic.h
  duplex_zlib.h compression.cc compression.h
  prng.h
  hash.cc hash.h
  util.cc util.h
  cvmfs_fsck.cc)

set (CVMFS_SWISSKNIFE_SOURCES
  options.cc options.h
  smalloc.h atomic.h
  platform.h platform_linux.h platform_osx.h
  duplex_zlib.h duplex_sqlite3.h
  logging.cc logging.h logging_internal.h
  globals.cc globals.h
  manifest.h manifest.cc
  whitelist.h whitelist.cc
  manifest_fetch.h manifest_fetch.cc
  prng.h
  hash.h hash.cc
  compression.h compression.cc
  util.h util.cc
  util_concurrency.h util_concurrency_impl.h util_concurrency.cc

  file_processing/async_reader.h file_processing/async_reader_impl.h file_processing/async_reader.cc
  file_processing/char_buffer.h
  file_processing/chunk.h file_processing/chunk.cc
  file_processing/chunk_detector.h file_processing/chunk_detector.cc
  file_processing/file.h file_processing/file.cc
  file_processing/io_dispatcher.h file_processing/io_dispatcher.cc
  file_processing/processor.h file_processing/processor.cc
  file_processing/file_processor.h file_processing/file_processor.cc

  pathspec/pathspec.h pathspec/pathspec.cc
  pathspec/pathspec_pattern.h pathspec/pathspec_pattern.cc

  upload.h upload.cc
  upload_spooler_definition.h upload_spooler_definition.cc
  upload_facility.h upload_facility.cc
  upload_local.h upload_local.cc
  s3fanout.h s3fanout.cc
  upload_s3.h upload_s3.cc
  upload_spooler_result.h
  json_document.h json_document.cc

  sanitizer.cc sanitizer.h
  dns.h dns.cc
  duplex_curl.h duplex_cares.h download.h download.cc
  signature.cc signature.h

  fs_traversal.h
  sync_item.h sync_item.cc
  sync_union.h sync_union.cc
  sync_mediator.h sync_mediator.cc

  file_chunk.h file_chunk.cc
  directory_entry.h directory_entry.cc
  shortstring.h
  catalog_traversal.h catalog_traversal.cc
  sql.h sql_impl.h sql.cc
  catalog_sql.h catalog_sql.cc
  catalog.h catalog.cc
  catalog_rw.h catalog_rw.cc
  catalog_mgr.h catalog_mgr.cc
  catalog_mgr_ro.h catalog_mgr_ro.cc
  catalog_mgr_rw.h catalog_mgr_rw.cc
  catalog_counters.h catalog_counters_impl.h catalog_counters.cc
  dirtab.h dirtab.cc
  history.h
  history_sql.h history_sql.cc
  history_sqlite.h history_sqlite.cc
  letter.h letter.cc

  swissknife_zpipe.h swissknife_zpipe.cc
  swissknife_check.h swissknife_check.cc
  swissknife_lsrepo.h swissknife_lsrepo.cc
  swissknife_pull.h swissknife_pull.cc
  swissknife_sign.h swissknife_sign.cc
  swissknife_letter.h swissknife_letter.cc
  swissknife_sync.h swissknife_sync.cc
  swissknife_info.h swissknife_info.cc
  swissknife_history.h swissknife_history.cc
  swissknife_migrate.h swissknife_migrate.cc
  swissknife_scrub.h swissknife_scrub.cc
  swissknife_gc.h swissknife_gc.cc
  swissknife.h swissknife.cc)


set (CVMFS_SWISSKNIFE_DEBUG_SOURCES
  ${CVMFS_SWISSKNIFE_SOURCES}
)

set (CVMFS_SUID_HELPER_SOURCES
  platform.h platform_osx.h platform_linux.h
  sanitizer.h sanitizer.cc
  cvmfs_suid_helper.cc
)

#
# configure some compiler flags for proper build
#

set (CVMFS_LOADER_CFLAGS "${CVMFS_LOADER_CFLAGS} -DCVMFS_CLIENT -DCVMFS_NAMESPACE_GUARD=loader")

set (CVMFS2_CFLAGS "${CVMFS2_CFLAGS} -DCVMFS_CLIENT -D_FILE_OFFSET_BITS=64 -fexceptions")
set (CVMFS2_DEBUG_CFLAGS "${CVMFS2_DEBUG_CFLAGS} ${CVMFS2_CFLAGS} -D_FILE_OFFSET_BITS=64 -O0 -DDEBUGMSG")

set (CVMFS_SWISSKNIFE_DEBUG_CFLAGS "${CVMFS_SWISSKNIFE_DEBUG_CFLAGS} -DDEBUGMSG -g")

set (CVMFS_FSCK_CFLAGS "${CVMFS_FSCK_CFLAGS} -DCVMFS_CLIENT")

set (LIBCVMFS_CFLAGS "${LIBCVMFS_CFLAGS} -D_FILE_OFFSET_BITS=64 -DCVMFS_CLIENT -DCVMFS_LIBCVMFS")

#
# set some include and linking stuff
#
set (CVMFS2_LD_FLAGS "${CVMFS2_LD_FLAGS} -ldl -lm")
include_directories (${INCLUDE_DIRECTORIES})

#
# do the job!
#
# for every target you can define a <targetname>_CFLAGS and <targetname>_LD_FLAGS
# variable to pass some target specific build options.
#

if (BUILD_CVMFS)

  add_executable (cvmfs2 ${CVMFS_LOADER_SOURCES})
  add_library (cvmfs_fuse_debug SHARED  ${CVMFS2_DEBUG_SOURCES})
  add_library (cvmfs_fuse SHARED ${CVMFS2_SOURCES})
  add_executable (cvmfs_fsck ${CVMFS_FSCK_SOURCES})

  if (LIBFUSE_BUILTIN)
    add_dependencies (cvmfs_fuse_debug libfuse) # here it does not matter if libfuse or osxfuse
    add_dependencies (cvmfs_fuse libfuse)       # at this point only the appropriate one is defined
  endif (LIBFUSE_BUILTIN)

  if (LIBCURL_BUILTIN)
    add_dependencies (cvmfs_fuse_debug libcares libcurl)
    add_dependencies (cvmfs_fuse libcares libcurl)
  endif (LIBCURL_BUILTIN)

  if (PACPARSER_BUILTIN)
    add_dependencies (cvmfs_fuse_debug pacparser)
    add_dependencies (cvmfs_fuse pacparser)
  endif (PACPARSER_BUILTIN)

  if (SQLITE3_BUILTIN)
    add_dependencies (cvmfs_fuse_debug sqlite3)
    add_dependencies (cvmfs_fuse sqlite3)
  endif (SQLITE3_BUILTIN)

  if (ZLIB_BUILTIN)
    add_dependencies (cvmfs_fuse_debug zlib)
    add_dependencies (cvmfs_fuse zlib)
    add_dependencies (cvmfs_fsck zlib)
  endif (ZLIB_BUILTIN)

  if (SPARSEHASH_BUILTIN)
    add_dependencies (cvmfs_fuse_debug sparsehash)
    add_dependencies (cvmfs_fuse sparsehash)
  endif (SPARSEHASH_BUILTIN)

  if (LEVELDB_BUILTIN)
    add_dependencies (cvmfs_fuse_debug libleveldb)
    add_dependencies (cvmfs_fuse libleveldb)
  endif (LEVELDB_BUILTIN)

  # set some properties
  set (CVMFS_LOADER_LD_FLAGS "${CVMFS_LOADER_LD_FLAGS} -ldl -lm")
  set_target_properties (cvmfs2 PROPERTIES COMPILE_FLAGS "${CVMFS_LOADER_CFLAGS}" LINK_FLAGS "${CVMFS_LOADER_LD_FLAGS}")
  set_target_properties (cvmfs_fuse_debug PROPERTIES COMPILE_FLAGS "${CVMFS2_DEBUG_CFLAGS}" LINK_FLAGS "${CVMFS2_LD_FLAGS} ${CVMFS2_DEBUG_LD_FLAGS}")
  set_target_properties (cvmfs_fuse PROPERTIES COMPILE_FLAGS "${CVMFS2_CFLAGS}" LINK_FLAGS "${CVMFS2_LD_FLAGS}")
  set_target_properties (cvmfs_fsck PROPERTIES COMPILE_FLAGS "${CVMFS_FSCK_CFLAGS}" LINK_FLAGS "${CVMFS_FSCK_LD_FLAGS}")

  set_target_properties (cvmfs_fuse PROPERTIES VERSION ${CernVM-FS_VERSION_STRING})
  set_target_properties (cvmfs_fuse_debug PROPERTIES VERSION ${CernVM-FS_VERSION_STRING})

  # link the stuff (*_LIBRARIES are dynamic link libraries *_archive are static link libraries ... one of them will be empty for each dependency)
  set (CVMFS_FUSE_LINK_LIBRARIES ${SQLITE3_LIBRARY} ${CURL_LIBRARIES} ${PACPARSER_LIBRARIES} ${ZLIB_LIBRARIES} ${LEVELDB_LIBRARIES} ${OPENSSL_LIBRARIES} ${FUSE_LIBRARIES} ${LIBFUSE_ARCHIVE} ${SQLITE3_ARCHIVE} ${LIBCURL_ARCHIVE} ${PACPARSER_ARCHIVE} ${LEVELDB_ARCHIVE} ${CARES_ARCHIVE} ${ZLIB_ARCHIVE} ${RT_LIBRARY} ${UUID_LIBRARIES} pthread dl)

  target_link_libraries (cvmfs2           ${CVMFS_LOADER_LIBS} ${OPENSSL_LIBRARIES} ${LIBFUSE} ${RT_LIBRARY} ${UUID_LIBRARIES} pthread dl)
  target_link_libraries (cvmfs_fuse_debug ${CVMFS2_DEBUG_LIBS} ${CVMFS_FUSE_LINK_LIBRARIES})
  target_link_libraries (cvmfs_fuse       ${CVMFS2_LIBS} ${CVMFS_FUSE_LINK_LIBRARIES})
  target_link_libraries (cvmfs_fsck       ${CVMFS_FSCK_LIBS} ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${ZLIB_ARCHIVE} pthread)

endif (BUILD_CVMFS)

if (BUILD_LIBCVMFS)
  # libcvmfs_only.a is a static lib of cvmfs without externals
  # libcvmfs.a includes the externals as well
  add_library (cvmfs_only STATIC ${LIBCVMFS_SOURCES})
  set (LIBCVMFS_LIBS ${LIBCVMFS_LIBS} ${CMAKE_CURRENT_BINARY_DIR}/libcvmfs_only.a)

  if (LIBCURL_BUILTIN)
    add_dependencies (cvmfs_only libcares libcurl)
    set (LIBCVMFS_LIBS ${CARES_ARCHIVE} ${LIBCVMFS_LIBS} )
    set (LIBCVMFS_LIBS ${LIBCURL_ARCHIVE} ${LIBCVMFS_LIBS} )
  endif (LIBCURL_BUILTIN)

  if (PACPARSER_BUILTIN)
    add_dependencies (cvmfs_only pacparser)
    set (LIBCVMFS_LIBS ${PACPARSER_ARCHIVE} ${LIBCVMFS_LIBS} )
  endif (PACPARSER_BUILTIN)

  if (SQLITE3_BUILTIN)
    add_dependencies (cvmfs_only sqlite3)
    set (LIBCVMFS_LIBS ${SQLITE3_ARCHIVE}  ${LIBCVMFS_LIBS} )
  endif (SQLITE3_BUILTIN)

  if (ZLIB_BUILTIN)
    add_dependencies (cvmfs_only zlib)
    set (LIBCVMFS_LIBS ${ZLIB_ARCHIVE} ${LIBCVMFS_LIBS} )
  endif (ZLIB_BUILTIN)

  if (SPARSEHASH_BUILTIN)
    add_dependencies (cvmfs_only sparsehash)
  endif (SPARSEHASH_BUILTIN)

  set_target_properties (cvmfs_only PROPERTIES COMPILE_FLAGS "${LIBCVMFS_CFLAGS}")

  if (NOT MACOSX)
    # We do not (yet) have a recipe for localizing private symbols
    # in Mac OS X, so skip that step on Mac
    set (LIBCVMFS_PUBLIC_SYMS "-public" "${CMAKE_CURRENT_SOURCE_DIR}/libcvmfs_public_syms.txt")
  endif (NOT MACOSX)

  add_custom_command(
    OUTPUT libcvmfs.a
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/combine_libs ${LIBCVMFS_PUBLIC_SYMS} libcvmfs.a ${LIBCVMFS_LIBS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${LIBCVMFS_LIBS}
  )

  # dummy target to cause merged libcvmfs to be produced
  add_custom_target (libcvmfs ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libcvmfs.a)
  add_dependencies (libcvmfs cvmfs_only)

  add_executable( test_libcvmfs ${TEST_LIBCVMFS_SOURCES} )
  target_link_libraries( test_libcvmfs ${CMAKE_CURRENT_BINARY_DIR}/libcvmfs.a ${SQLITE3_LIBRARY} ${CURL_LIBRARIES} ${PACPARSER_LIBRARIES} ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${RT_LIBRARY} ${UUID_LIBRARIES} pthread dl )
  add_dependencies (test_libcvmfs libcvmfs)

endif (BUILD_LIBCVMFS)

if (BUILD_SERVER)
  # create targets with corresponding sources and static link libraries
  add_executable (cvmfs_swissknife ${CVMFS_SWISSKNIFE_SOURCES})
  add_executable (cvmfs_suid_helper ${CVMFS_SUID_HELPER_SOURCES})
  add_custom_target (geoip ALL)

  if (GEOIP_BUILTIN)
    add_dependencies (python-geoip libgeoip)
    add_dependencies (geoip python-geoip)
  endif (GEOIP_BUILTIN)

  include (${VJSON_BUILTIN_LOCATION}/CVMFS-CMakeLists.txt)
  add_dependencies (cvmfs_swissknife libvjson)
  set (INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${VJSON_BUILTIN_LOCATION}/src)
  include_directories (${INCLUDE_DIRECTORIES})

  if (LIBCURL_BUILTIN)
    add_dependencies (cvmfs_swissknife libcares libcurl)
  endif (LIBCURL_BUILTIN)

  if (SQLITE3_BUILTIN)
    add_dependencies (cvmfs_swissknife sqlite3)
  endif (SQLITE3_BUILTIN)

  if (ZLIB_BUILTIN)
    add_dependencies (cvmfs_swissknife zlib)
  endif (ZLIB_BUILTIN)

  if (TBB_PRIVATE_LIB)
    add_dependencies (cvmfs_swissknife libtbb)
  endif (TBB_PRIVATE_LIB)

  set_target_properties (cvmfs_swissknife PROPERTIES COMPILE_FLAGS "${CVMFS_SWISSKNIFE_CFLAGS}" LINK_FLAGS "${CVMFS_SWISSKNIFE_LD_FLAGS}")

  # link the stuff (*_LIBRARIES are dynamic link libraries)
  target_link_libraries (cvmfs_swissknife  ${CVMFS_SWISSKNIFE_LIBS} ${SQLITE3_LIBRARY} ${CURL_LIBRARIES} ${ZLIB_LIBRARIES} ${TBB_LIBRARIES} ${OPENSSL_LIBRARIES} ${LIBCURL_ARCHIVE} ${CARES_ARCHIVE} ${SQLITE3_ARCHIVE} ${ZLIB_ARCHIVE} ${RT_LIBRARY} ${VJSON_ARCHIVE} pthread dl)

  if (BUILD_SERVER_DEBUG)
    add_executable (cvmfs_swissknife_debug ${CVMFS_SWISSKNIFE_DEBUG_SOURCES})

    if (LIBCURL_BUILTIN)
      add_dependencies (cvmfs_swissknife_debug libcares libcurl)
    endif (LIBCURL_BUILTIN)

    if (SQLITE3_BUILTIN)
      add_dependencies (cvmfs_swissknife_debug sqlite3)
    endif (SQLITE3_BUILTIN)

    if (ZLIB_BUILTIN)
      add_dependencies (cvmfs_swissknife_debug zlib)
    endif (ZLIB_BUILTIN)

    if (TBB_PRIVATE_LIB)
      add_dependencies (cvmfs_swissknife_debug libtbb)
    endif (TBB_PRIVATE_LIB)

    set_target_properties (cvmfs_swissknife_debug PROPERTIES COMPILE_FLAGS "${CVMFS_SWISSKNIFE_DEBUG_CFLAGS}" LINK_FLAGS "${CVMFS_SWISSKNIFE_DEBUG_LD_FLAGS}")

    # link the stuff (*_LIBRARIES are dynamic link libraries)
    if (NOT TBB_DEBUG_LIBRARIES)
      message (WARNING "Debug libraries of TBB were not found. Using the release versions instead.")
      set (TBB_DEBUG_LIBRARIES ${TBB_LIBRARIES})
    endif (NOT TBB_DEBUG_LIBRARIES)
    target_link_libraries (cvmfs_swissknife_debug  ${CVMFS_SWISSKNIFE_LIBS} ${SQLITE3_LIBRARY} ${CURL_LIBRARIES} ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${LIBCURL_ARCHIVE} ${CARES_ARCHIVE} ${SQLITE3_ARCHIVE} ${ZLIB_ARCHIVE} ${TBB_DEBUG_LIBRARIES} ${RT_LIBRARY} ${VJSON_ARCHIVE} pthread dl)
  endif (BUILD_SERVER_DEBUG)
endif (BUILD_SERVER)

#
# installation
#

if (BUILD_CVMFS)
  install (
    TARGETS      cvmfs2 cvmfs_fsck
    RUNTIME
    DESTINATION    bin
  )

  install (
    TARGETS      cvmfs_fuse cvmfs_fuse_debug
    LIBRARY
    DESTINATION    ${CMAKE_INSTALL_LIBDIR}
  )

  install (
    FILES      cvmfs_talk cvmfs_config
    DESTINATION    bin
    PERMISSIONS    OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  )

  if(INSTALL_BASH_COMPLETION)
    install (
        FILES         bash_completion/cvmfs.bash_completion
        RENAME        cvmfs
        DESTINATION   /etc/bash_completion.d
        PERMISSIONS   OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    )
  endif (INSTALL_BASH_COMPLETION)

endif (BUILD_CVMFS)

if (BUILD_LIBCVMFS)
  install (
    FILES      ${CMAKE_CURRENT_BINARY_DIR}/libcvmfs.a
    DESTINATION    ${CMAKE_INSTALL_LIBDIR}
  )
  install (
    FILES                   libcvmfs.h
    DESTINATION             include
  )
endif (BUILD_LIBCVMFS)

if (BUILD_SERVER)
  install (
    TARGETS     cvmfs_swissknife
    RUNTIME
    DESTINATION    bin
  )

  install (
    TARGETS     cvmfs_suid_helper
    RUNTIME
    DESTINATION    bin
    PERMISSIONS    OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE SETUID
  )

  if (BUILD_SERVER_DEBUG)
    install (
      TARGETS     cvmfs_swissknife_debug
      RUNTIME
      DESTINATION   bin
    )
  endif (BUILD_SERVER_DEBUG)

  install (
    FILES      cvmfs_server
    DESTINATION    bin
    PERMISSIONS    OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  )

  install(
    FILES      cvmfs_server_hooks.sh.demo
    DESTINATION    "/etc/cvmfs"
    PERMISSIONS    OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  )

  install(
    FILES      webapi/cvmfs-api.wsgi
    DESTINATION    "/var/www/wsgi-scripts"
    PERMISSIONS    OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  )

  if (GEOIP_BUILTIN)
    install(
      FILES      ${PYTHON_GEOIP_BUILTIN_LOCATION}/dist/GeoIP.so
      DESTINATION    "/usr/share/cvmfs-server/webapi"
      PERMISSIONS    OWNER_READ GROUP_READ WORLD_READ
    )
  endif (GEOIP_BUILTIN)

  install(
    FILES      webapi/cvmfs_api.py webapi/cvmfs_geo.py
    DESTINATION    "/usr/share/cvmfs-server/webapi"
    PERMISSIONS    OWNER_READ GROUP_READ WORLD_READ
  )

  install(
    FILES          doc/README-spooler
    DESTINATION    "/var/spool/cvmfs"
    PERMISSIONS    OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    RENAME         README
  )

  install(
    CODE  "FILE(MAKE_DIRECTORY \$ENV{DESTDIR}/var/lib/cvmfs-server/geo)"
  )

endif (BUILD_SERVER)
