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


add_executable(dsdcons src_d/dscons.c)

target_link_libraries(dsdcons dsnet)


add_executable(dsmcons src_m/dscons.c)

target_link_libraries(dsmcons dsnet)


add_executable(dsecons src_e/dscons.c)

target_link_libraries(dsecons dsnet)


add_executable(dsicons src_i/dscons.c)

target_link_libraries(dsicons dsnet)


add_executable(dsefilesv src_e/dsfilesv.c)

target_link_libraries(dsefilesv dsnet)


add_executable(dsifilesv src_i/dsfilesv.c)

target_link_libraries(dsifilesv dsnet)


add_executable(dseping src_e/dsping.c)

target_link_libraries(dseping dsnet)


add_executable(dsiping src_i/dsping.c)

target_link_libraries(dsiping dsnet)


add_executable(dsestart src_e/dsstart.c)

target_link_libraries(dsestart dsnet)


add_executable(dsistart src_i/dsstart.c)

target_link_libraries(dsistart dsnet)


add_executable(dselist src_e/dslist.c)

target_link_libraries(dselist dsnet)


add_executable(dsidb
	dbg_i/dbg.c
	dbg_i/exe.c
	dbg_i/help.c
	dbg_i/ireg.c
	dbg_i/mdebug.c
	dbg_i/mem.c
	dbg_i/mod.c
	dbg_i/opc.c
	dbg_i/sym.c
)

target_link_libraries(dsidb dsnet)


add_executable(dsedb
	dbg_e/dbg.c
	dbg_e/ereg.c
	dbg_e/ethread.c
	dbg_e/exe.c
	dbg_e/help.c
	dbg_e/mdebug.c
	dbg_e/mem.c
	dbg_e/mod.c
	dbg_e/opc.c
	dbg_e/sym.c
	dbg_e/vopc.c
)

target_link_libraries(dsedb dsnet)

