
function(git_version)
    # Возвращает короткую запись SHA последнего коммита
    # git log --pretty=%h -1
    execute_process( COMMAND git log --format=%h -1
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                     OUTPUT_VARIABLE GIT_VERSION_SHA
                     OUTPUT_STRIP_TRAILING_WHITESPACE )
    # Возвращает имя ветки или (если есть) тэг
    # git name-rev --name-only HEAD
    execute_process( COMMAND git name-rev --name-only ${GIT_VERSION_SHA}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                     OUTPUT_VARIABLE GIT_VERSION_BRANCH
                     OUTPUT_STRIP_TRAILING_WHITESPACE )
    # Удаляем префикс и постфикс, если это тэг.
    string( REGEX REPLACE "remotes/origin/" "" GIT_VERSION_BRANCH ${GIT_VERSION_BRANCH} )
    string( REGEX REPLACE "tags/" "" GIT_VERSION_BRANCH ${GIT_VERSION_BRANCH} )
    string( REGEX REPLACE "\\^0" "" GIT_VERSION_BRANCH ${GIT_VERSION_BRANCH} )
    set( GIT_VERSION_STRING "${GIT_VERSION_BRANCH}-${GIT_VERSION_SHA}" PARENT_SCOPE )

endfunction(git_version)
