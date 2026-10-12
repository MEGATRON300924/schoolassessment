# Assessment requirement map

- **OOP and namespaces:** `Model::PatientRecord` is abstract, has a virtual destructor and three derived patient categories. `Model::Appointable` defines the appointment interface. Code is separated into Model, Controller, UI and Utils folders/namespaces.
- **Data structures:** patient records use `std::vector<std::shared_ptr<PatientRecord>>`; the consultation waiting room uses `std::queue`; administrative undo uses `std::stack<AdminCommand>`; the recent patient cache is a fixed `std::array<std::string, 10>`.
- **Manual algorithms:** `Utils/Algorithms.cpp` implements linear ID search, binary ID search (input must be ID-sorted), recursive name search, quick sort and merge sort. No standard sorting/search algorithm is used for these required algorithms.
- **GUI:** Qt Widgets tabs cover patient records, appointments, waiting queue, search/sort and reports. The interface uses layout managers, highlights emergency patients, validates age and duplicate IDs, refreshes the queue view on a timer and supports Ctrl+S, Ctrl+F and Ctrl+Q.
- **Persistence/errors:** JSON data is loaded on startup and saved on exit or Ctrl+S. Custom age, duplicate-ID and file I/O exceptions are provided.

## Important limitations to understand before submission

This is an educational application, not a medical-grade clinical system. The current JSON persistence covers patients and appointments; the in-memory waiting queue and undo history are session-only. The queue uses FIFO order rather than automatically prioritising emergency patients. Appointment date validation and notification delivery are not implemented. The report shows category counts and pending appointments, not a full historical daily audit. Review these trade-offs against your lecturer's marking rubric and explain the design in your own words.
