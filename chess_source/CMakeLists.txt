cmake_minimum_required(VERSION 3.16)
project(progettopf VERSION 0.1.0)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast")

# abilita asserzioni di debug (in gcc), l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")

# richiedi il componente graphics della libreria SFML, versione 2.5
#   le dipendenze vengono identificate automaticamente
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(boids main.cpp boids.cpp flight_laws.cpp statistics.cpp)
target_link_libraries(boids PRIVATE sfml-graphics)

# se il testing e' abilitato...
#   per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_TESTING)
  # aggiungi l'eseguibile all.t
  add_executable(boids.t boids.test.cpp boids.cpp flight_laws.cpp statistics.cpp) 
  add_executable(flight_laws.t flight_laws.test.cpp flight_laws.cpp boids.cpp statistics.cpp)
  add_executable(statistics.t statistics.test.cpp statistics.cpp boids.cpp flight_laws.cpp)

  # aggiungi l'eseguibile all.t alla lista dei test
  target_link_libraries(boids.t PRIVATE sfml-graphics)
  target_link_libraries(flight_laws.t PRIVATE sfml-graphics)
  target_link_libraries(statistics.t PRIVATE sfml-graphics)
  
  # add_test(NAME all.t COMMAND all.t)
  add_test(NAME copy_image
         COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/pidgey.png ${CMAKE_BINARY_DIR}/pidgey.png)
  add_test(NAME boids.t COMMAND boids.t)
  add_test(NAME flight_laws.t COMMAND flight_laws.t)
  add_test(NAME statistics.t COMMAND statistics.t)
  add_test(NAME cleanup_image
         COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/pidgey.png)
endif()