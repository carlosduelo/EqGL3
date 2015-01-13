

set(EQGL3_PACKAGE_VERSION 0.1)
set(EQGL3_DEPENDS
	REQUIRED Equalizer)
set(EQGL3_REPO_URL https://github.com/carlosduelo/EqGL3.git)
set(EQGL3_REPO_TAG master)
set(EQGL3_FORCE_BUILD ${CI_BUILD})

if(CI_BUILD_COMMIT)
  set(EQGL3_REPO_TAG ${CI_BUILD_COMMIT})
else()
  set(EQGL3_REPO_TAG master)
endif()
set(EQGL3_FORCE_BUILD ON)
set(EQGL3_SOURCE ${CMAKE_SOURCE_DIR})