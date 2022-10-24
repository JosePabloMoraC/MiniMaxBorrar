set(HEADER_LIST "${DotsAndBoxes_SOURCE_DIR}/include/*.hh")

add_executable(DotsAndBoxes main.cc Board.cc Cell.cc MiniMax.cc Movement.cc Player.cc)

target_include_directories(
    DotsAndBoxes
    PUBLIC
    "${DotsAndBoxes_SOURCE_DIR}/include")