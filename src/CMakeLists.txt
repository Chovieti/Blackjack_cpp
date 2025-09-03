cmake_minimum_required(VERSION 3.5)
project(blackjack)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(blackjack
    main.cc
    Card.cc
    CardWidget.cc
    GameTable.cc
    Hand.cc
    Deck.cc
)

add_compile_options(-Wall -Wextra)

target_link_libraries(blackjack
    PRIVATE
    Qt6::Widgets
)