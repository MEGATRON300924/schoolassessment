#include "Algorithms.h"
#include <cctype>
namespace Utils {
namespace {
std::string lower(std::string s) { for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c))); return s; }
bool less(const std::shared_ptr<Model::PatientRecord>& a, const std::shared_ptr<Model::PatientRecord>& b, SortKey k) {
    if (k == SortKey::ID) return a->patientID() < b->patientID();
    if (k == SortKey::Age) return a->age() < b->age();
    return lower(a->name()) < lower(b->name());
}
void quick(Patients& a, int lo, int hi, SortKey k) {
    int i=lo, j=hi; auto pivot=a[static_cast<std::size_t>(lo+(hi-lo)/2)];
    while(i<=j) { while(less(a[static_cast<std::size_t>(i)],pivot,k)) ++i; while(less(pivot,a[static_cast<std::size_t>(j)],k)) --j;
        if(i<=j) { auto t=a[static_cast<std::size_t>(i)]; a[static_cast<std::size_t>(i)]=a[static_cast<std::size_t>(j)]; a[static_cast<std::size_t>(j)]=t; ++i; --j; } }
    if(lo<j) quick(a,lo,j,k); if(i<hi) quick(a,i,hi,k);
}
void merge(Patients& a, Patients& tmp, int lo, int mid, int hi, SortKey k) {
    int i=lo,j=mid+1,n=lo;
    while(i<=mid && j<=hi) tmp[static_cast<std::size_t>(n++)]=less(a[static_cast<std::size_t>(j)],a[static_cast<std::size_t>(i)],k)?a[static_cast<std::size_t>(j++)]:a[static_cast<std::size_t>(i++)];
    while(i<=mid) tmp[static_cast<std::size_t>(n++)]=a[static_cast<std::size_t>(i++)];
    while(j<=hi) tmp[static_cast<std::size_t>(n++)]=a[static_cast<std::size_t>(j++)];
    for(int x=lo;x<=hi;++x) a[static_cast<std::size_t>(x)]=tmp[static_cast<std::size_t>(x)];
}
void msort(Patients& a, Patients& tmp, int lo, int hi, SortKey k) { if(lo>=hi)return; int mid=lo+(hi-lo)/2; msort(a,tmp,lo,mid,k); msort(a,tmp,mid+1,hi,k); merge(a,tmp,lo,mid,hi,k); }
}
int linearSearchById(const Patients& a,const std::string& id){for(std::size_t i=0;i<a.size();++i)if(a[i]->patientID()==id)return static_cast<int>(i);return -1;}
int binarySearchById(const Patients& a,const std::string& id){int l=0,r=static_cast<int>(a.size())-1;while(l<=r){int m=l+(r-l)/2;const auto& v=a[static_cast<std::size_t>(m)]->patientID();if(v==id)return m;if(v<id)l=m+1;else r=m-1;}return -1;}
int recursiveSearchByName(const Patients& a,const std::string& name,int i){if(i>=static_cast<int>(a.size()))return -1;if(lower(a[static_cast<std::size_t>(i)]->name()).find(lower(name))!=std::string::npos)return i;return recursiveSearchByName(a,name,i+1);}
void quickSort(Patients& a,SortKey k){if(a.size()>1)quick(a,0,static_cast<int>(a.size())-1,k);}
void mergeSort(Patients& a,SortKey k){if(a.size()>1){Patients tmp(a.size());msort(a,tmp,0,static_cast<int>(a.size())-1,k);}}
}
