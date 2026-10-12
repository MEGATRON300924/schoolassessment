# Smart Clinic Appointment & Patient Management System (SCAPMS)

A modular C++17/Qt Widgets application for managing clinic patients, appointments, a consultation queue, and reports.

## Requirements
- CMake 3.16+
- C++17 compiler
- Qt 6 Widgets (Qt 5.15 is also supported)

## Build

```bash
cmake -S . -B build
cmake --build build --config Release
```

Run `build/SCAPMS` (or `build/Release/SCAPMS.exe` on multi-configuration Windows generators).

The application stores its JSON data in `scapms-data.json` in the current working directory. Back up that file regularly; it contains patient information. This educational demo is not a certified clinical record system and should not be used for real patient care without a security, privacy, and clinical-safety review.

## Features
- Polymorphic patient records (general, emergency, follow-up)
- Appointment scheduling/cancellation and consultation waiting queue
- Manual linear, binary, and recursive search; hand-written quick sort and merge sort
- Undo for supported administrative operations
- JSON persistence, validation, reports, keyboard shortcuts, and timed refresh
