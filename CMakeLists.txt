cmake_minimum_required(VERSION 3.6)
project(dsnet)

set(GIT_LOG_COMMAND
	git -C ${CMAKE_CURRENT_SOURCE_DIR} log -1 $'\\x2d\\x2d\\x66\\x6f\\x72\\x6d\\x61\\x74\\x3d\\x25\\x61\\x74\\x2d\\x25\\x28\\x64\\x65\\x73\\x63\\x72\\x69\\x62\\x65\\x3a\\x74\\x61\\x67\\x73\\x2c\\x61\\x62\\x62\\x72\\x65\\x76\\x3d\\x30\\x29\\x2d\\x25\\x68'
)

if(NOT DEFINED ${DSNET_VERSION})
execute_process(
    COMMAND ${CMAKE_COMMAND} -E echo ${GIT_LOG_COMMAND}
    COMMAND bash
    OUTPUT_VARIABLE DSNET_VERSION
)

string(STRIP ${DSNET_VERSION} DSNET_VERSION)
endif()

add_definitions(
	-DDSNET_VERSION_STRING="${DSNET_VERSION}"
	-DDSNET_STAMP_STRING="Version ${DSNET_VERSION} https://github.com/ps2dbg/dsnet"
	-Werror=implicit-function-declaration
)

if(WIN32)
add_link_options(-static -static-libgcc)
endif()

include_directories(include)
set(DSNET_LIB_SRC
	lib/cmds.c
	lib/dcmp.c
	lib/deci2.c
	lib/desc.c
	lib/drfp.c
	lib/dump.c
	lib/editline.c
	lib/expr.c
	lib/file.c
	lib/fmt.c
	lib/kbd.c
	lib/log.c
	lib/mem.c
	lib/misc.c
	lib/net.c
	lib/netmp.c
	lib/pa.c
	lib/printf.c
	lib/psnet.c
	lib/quad.c
	lib/scans.c
	lib/subst.c
	lib/check_pfm.c
	lib/check_gif.c
	lib/check_sss.c
)
add_library(dsnet STATIC ${DSNET_LIB_SRC})
if(WIN32)
target_link_libraries(dsnet -lws2_32)
endif()

add_executable(dscomp src/dscomp.c)

target_link_libraries(dscomp dsnet)

add_executable(dskill src/dskill.c)

target_link_libraries(dskill dsnet)

add_executable(dsreset src/dsreset.c)

target_link_libraries(dsreset dsnet)


add_executable(dsnetm src/dsnetm.c)

target_link_libraries(dsnetm dsnet)


add_executable(dssim src/dssim.c)

target_link_libraries(dssim dsnet)


add_executable(dsstat src/dsstat.c)

target_link_libraries(dsstat dsnet)


add_executable(pacons src/pacons.c)

target_link_libraries(pacons dsnet)


add_executable(dsdcons src/dscons.c)

target_link_libraries(dsdcons dsnet)
target_compile_definitions(dsdcons PRIVATE DSNET_COMPILING_D)


add_executable(dsmcons src/dscons.c)

target_link_libraries(dsmcons dsnet)
target_compile_definitions(dsmcons PRIVATE DSNET_COMPILING_M)


add_executable(dsecons src/dscons.c)

target_link_libraries(dsecons dsnet)
target_compile_definitions(dsecons PRIVATE DSNET_COMPILING_E)


add_executable(dsicons src/dscons.c)

target_link_libraries(dsicons dsnet)
target_compile_definitions(dsicons PRIVATE DSNET_COMPILING_I)


add_executable(dsefilesv src/dsfilesv.c)

target_link_libraries(dsefilesv dsnet)
target_compile_definitions(dsefilesv PRIVATE DSNET_COMPILING_E)


add_executable(dsifilesv src/dsfilesv.c)

target_link_libraries(dsifilesv dsnet)
target_compile_definitions(dsifilesv PRIVATE DSNET_COMPILING_I)


add_executable(dseping src/dsping.c)

target_link_libraries(dseping dsnet)
target_compile_definitions(dseping PRIVATE DSNET_COMPILING_E)


add_executable(dsiping src/dsping.c)

target_link_libraries(dsiping dsnet)
target_compile_definitions(dsiping PRIVATE DSNET_COMPILING_I)


add_executable(dsestart src/dsstart.c)

target_link_libraries(dsestart dsnet)
target_compile_definitions(dsestart PRIVATE DSNET_COMPILING_E)


add_executable(dsistart src/dsstart.c)

target_link_libraries(dsistart dsnet)
target_compile_definitions(dsistart PRIVATE DSNET_COMPILING_I)


add_executable(dselist src/dslist.c)

target_link_libraries(dselist dsnet)
target_compile_definitions(dselist PRIVATE DSNET_COMPILING_E)


add_executable(dsidb
	dbg/dbg.c
	dbg/exe.c
	dbg/help.c
	dbg/ireg.c
	dbg/mdebug.c
	dbg/mem.c
	dbg/mod.c
	dbg/opc.c
	dbg/sym.c
)

target_link_libraries(dsidb dsnet)
target_compile_definitions(dsidb PRIVATE DSNET_COMPILING_I)


add_executable(dsedb
	dbg/dbg.c
	dbg/ereg.c
	dbg/ethread.c
	dbg/exe.c
	dbg/help.c
	dbg/mdebug.c
	dbg/mem.c
	dbg/mod.c
	dbg/opc.c
	dbg/sym.c
	dbg/vopc.c
)

target_link_libraries(dsedb dsnet)
target_compile_definitions(dsedb PRIVATE DSNET_COMPILING_E)

