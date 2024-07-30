cmake_minimum_required(VERSION 3.22)
project(RISC5)

set(CMAKE_CXX_STANDARD 17)

add_executable(RISC5 main.cpp
        src/ALU.cpp
        src/ALU.h
        src/cpu.cpp
        src/cpu.h
        src/decoder.cpp
        src/decoder.h
        src/instruction.cpp
        src/instruction.h
        src/Load_Store_Buffer.cpp
        src/Load_Store_Buffer.h
        src/memory.cpp
        src/memory.h
        src/Register_File.cpp
        src/Register_File.h
        src/Reservation_Station.cpp
        src/Reservation_Station.h
        src/Reorder_buffer.cpp
        src/Reorder_buffer.h
        util/utils.cpp
        util/utils.h
        util/config.h
        util/CircularQueue.cpp
        util/CircularQueue.h
    )
