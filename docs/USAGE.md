# Usage guide

## Input files

The application works with:

1. A blurred image.
2. A CSV file with inertial sensor readings.

Supported image formats:

- JPEG
- PNG
- BMP

Expected accelerometer CSV format:

```csv
ax,ay,az,timestamp_ns
-1.7621863,5.559506,8.341654,126317664071857
-1.7621863,5.559506,8.341654,126317664048857
```

If your application version expects CSV without headers, delete the first line.

## Workflow

1. Launch the application.
2. Load a blurred image.
3. Load accelerometer CSV data.
4. Configure shooting parameters:
   - shutter speed;
   - gravity acceleration value;
   - PSF scale;
   - smoothing;
   - number of Richardson-Lucy iterations.
5. Select a fast preview method:
   - Tikhonov regularization;
   - Wiener filter.
6. Build the PSF.
7. Run the restoration algorithm.
8. Compare the restored image with the original image.

## Recommended sensor recording setup

For collecting sensor data, use an Android camera application that can record video/images and IMU data synchronously, for example OpenCamera Sensors.

Recommended settings:

- enable accelerometer data recording;
- use the maximum available accelerometer sampling frequency;
- keep the image and CSV file from the same capture session together.

## Notes

- The algorithm is sensitive to the quality of sensor readings.
- Incorrect shutter speed or PSF scale can noticeably reduce restoration quality.
- Too many Richardson-Lucy iterations may increase noise and ringing artifacts.
