#pragma once
#include "model/PatientRecord.h"
#include <memory>
#include <string>
#include <vector>
namespace Utils {
using Patients = std::vector<std::shared_ptr<Model::PatientRecord>>;
int linearSearchById(const Patients&, const std::string&);
int binarySearchById(const Patients&, const std::string&); // records must be sorted by ID
int recursiveSearchByName(const Patients&, const std::string&, int index = 0);
enum class SortKey { Name, ID, Age };
void quickSort(Patients&, SortKey);
void mergeSort(Patients&, SortKey);
}
