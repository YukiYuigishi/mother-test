#pragma once
#include <cstdint>

namespace Ps3Decoder {
    struct Data {
        int8_t rx;
        int8_t ry;
        int8_t lx;
        int8_t ly;
        bool circle;
        bool triangle;
        bool cross;
        bool square;
        bool right;
        bool left;
        bool up;
        bool down;
        bool r1;
        bool r2;
        bool l1;
        bool l2;
        bool select;
        bool start;
        int8_t battery;
    };

    inline Data decode(uint8_t data[12]) {
        int8_t rx_upper;
        int8_t ry_upper;
        int8_t lx_upper;
        int8_t ly_upper;
        int8_t rx_lower;
        int8_t ry_lower;
        int8_t lx_lower;
        int8_t ly_lower;
        bool circle;
        bool triangle;
        bool cross;
        bool square;
        bool right;
        bool left;
        bool up;
        bool down;
        bool r1;
        bool r2;
        bool l1;
        bool l2;
        bool select;
        bool start;
        int8_t battery;
        for (int i = 0; i < 12; i++) {
            switch (data[i] & 0b00001111) {
            case 0b00000000:
                rx_upper = data[i] & 0b11110000;
                break;
            case 0b00000001:
                rx_lower = data[i] >> 4;
                break;
            case 0b00000010:
                ry_upper = data[i] & 0b11110000;
                break;
            case 0b00000011:
                ry_lower = data[i] >> 4;
                break;
            case 0b00000100:
                lx_upper = data[i] & 0b11110000;
                break;
            case 0b00000101:
                lx_lower = data[i] >> 4;
                break;
            case 0b00000110:
                ly_upper = data[i] & 0b11110000;
                break;
            case 0b00000111:
                ly_lower = data[i] >> 4;
                break;
            case 0b00001000:
                circle = data[i] & 0b10000000;
                triangle = data[i] & 0b01000000;
                cross = data[i] & 0b00100000;
                square = data[i] & 0b00010000;
                break;
            case 0b00001001:
                right = data[i] & 0b10000000;
                left = data[i] & 0b01000000;
                up = data[i] & 0b00100000;
                down = data[i] & 0b00010000;
                break;
            case 0b00001010:
                r1 = data[i] & 0b10000000;
                r2 = data[i] & 0b01000000;
                l1 = data[i] & 0b00100000;
                l2 = data[i] & 0b00010000;
                break;
            case 0b00001011:
                select = data[i] & 0b10000000;
                start = data[i] & 0b01000000;
                battery = (data[i] &0b00110000) >> 4;
                break;
            case 0b00001100:
            case 0b00001101:
            case 0b00001110:
            case 0b00001111:
            default:
                // pass
                break;
            }
        }
        int8_t rx = rx_upper | rx_lower;
        int8_t ry = ry_upper | ry_lower;
        int8_t lx = lx_upper | lx_lower;
        int8_t ly = ly_upper | ly_lower;
        return {
            rx, ry, lx, ly,
            circle, triangle, cross, square,
            right, left, up, down,
            r1, r2, l1, l2,
            select, start, battery
        };
    }
} // namespace Ps3Decoder
