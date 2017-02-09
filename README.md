# ILI9341_ESP32
A fast driver for displays with ILI9341 controller to be used with an ESP32.

The ESP32 can transfer 64Bytes at once to the SPI bus instead of sending byte for byte. This driver takes usage of that.

This driver is a work in progress. It will provide an interface fully compatible to Adafruits ILI9341 and GFX library.

Expect changes and additions regularly.

## You want to help? Great.

Any help is welcome. At the moment the most important thing to do is to check if everything is pixel-correct.
If you find a bug or think that there are things that can be done smarter, faster or smaller, let me know.


## Speed comparison

| Benchmark | this driver | this driver with HAL-Locks disabled | Adafruits driver with disabled HAL-Locks and HW-SPI |
| --- | --- | --- | --- |
| Screen Fill             |     189280 |  104626 (faster) |   662772 (slower) |
| Text                    |      70418 |   12330 (faster) |    51971 (faster) |
| Lines                   |     667364 |  105083 (faster) |   511414 (faster) |
| Horz/Vert Lines         |      21896 |    9329 (faster) |    55201 (slower) |
| Rectangles (outline)    |      17355 |    6336 (faster) |    35831 (slower) |
| Rectangles (filled)     |     395414 |  217453 (faster) |  1376127 (slower) |
| Circles (filled)        |     312178 |   66635 (faster) |   235643 (faster) |
| Circles (outline)       |     675115 |  100625 (faster) |   223025 (faster) |
| Triangles (outline)     |     147280 |   32708 (faster) |   162235 (slower) |
| Triangles (filled)      |     303023 |   91075 (faster) |   470071 (slower) |
| Rounded Rects (outline) |     185902 |   30606 (faster) |    91688 (faster) |
| Rounded Rects (filled)  |     512880 |  250357 (faster) |  1513809 (slower) |
