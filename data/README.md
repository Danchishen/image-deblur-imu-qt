# Example data

This folder is intended for small example files that help demonstrate how the application works.

Recommended files:

```text
data/example_blurred_image.png
data/example_accelerometer.csv
```

Do not upload large raw videos or big datasets to GitHub. Keep only small examples that are useful for testing and demonstration.

## CSV format

Expected accelerometer CSV structure:

```csv
ax,ay,az,timestamp_ns
-1.7621863,5.559506,8.341654,126317664071857
-1.7621863,5.559506,8.341654,126317664048857
```

If the current application version expects CSV without a header, remove the first line.
