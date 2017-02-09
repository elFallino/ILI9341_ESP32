# ILI9341_ESP32
A fast driver for displays with ILI9341 controller to be used with an ESP32.

The ESP32 can transfer 64Bytes at once to the SPI bus instead of sending byte for byte. This driver takes usage of that.

This driver is a work in progress. It will provide an interface fully compatible to Adafruits ILI9341 and GFX library.

Expect changes and additions regularly.

## You want to help? Great.

Any help is welcome. At the moment the most important thing to do is to check if everything is pixel-correct.
If you find a bug or think that there are things that can be done smarter, faster or smaller, let me know.


## Speed comparison

| Benchmark | this driver | Adafruits driver with disabled HAL-Locks and HW-SPI |
| --- | --- | --- | --- |
| Screen Fill             |     189517 |   662772 (slower) |
| Text                    |     131489 |    51971 (faster) |
| Lines                   |    1296059 |   511414 (faster) |
| Horz/Vert Lines         |      27391 |    55201 (slower) |
| Rectangles (outline)    |      25048 |    35831 (slower) |
| Rectangles (filled)     |     397414 |  1376127 (slower) |
| Circles (filled)        |     580863 |   235643 (faster) |
| Circles (outline)       |    1310896 |   223025 (faster) |
| Triangles (outline)     |     282804 |   162235 (faster) |
| Triangles (filled)      |     444575 |   470071 (slower) |
| Rounded Rects (outline) |     351972 |    91688 (faster) |
| Rounded Rects (filled)  | 4277669007 |  1513809 (faster or meas. error) |
