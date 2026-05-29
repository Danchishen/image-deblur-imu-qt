# Resume project description

## Short version

Developed a C++/Qt desktop application for restoring motion-blurred images using smartphone inertial sensor data. The application loads an image and accelerometer CSV data, estimates camera displacement, builds a PSF kernel, and restores the image using deconvolution algorithms.

## CV bullet points

- Developed a desktop C++/Qt application for restoring motion-blurred images using accelerometer data.
- Implemented PSF construction from camera displacement calculated from inertial sensor readings.
- Implemented and compared deconvolution methods, including Tikhonov regularization, Wiener filtering, and Richardson-Lucy restoration.
- Added a Qt Widgets graphical interface for loading images, importing CSV sensor data, configuring parameters, previewing results, and comparing original/restored images.
- Used FFTW for Fourier-transform-based image processing.

## Russian version for resume

Разработал C++/Qt-приложение для восстановления смазанных изображений с использованием данных инерциальных датчиков смартфона. Программа загружает изображение и CSV-файл с показаниями акселерометра, оценивает смещение камеры, строит PSF-функцию и выполняет восстановление изображения методами деконволюции. Реализованы предпросмотр результата, настройка параметров обработки и сравнение исходного и восстановленного изображения.

## Technologies

C++, Qt, Qt Widgets, FFTW, CMake/qmake, image processing, deconvolution, CSV processing.
