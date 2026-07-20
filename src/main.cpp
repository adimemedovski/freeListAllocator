#include "freeListAllocator.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

typedef struct {
    char *name;
    size_t age;
} Person; 

typedef struct {
    Person *persons;
} People;

//bool initPeople(FreeList *freeList, People *people) {
    //people -> persons = (Person*) freeListAlloc(freeList, sizeof(Person) * 2, _Alignof(People));
    
    //if (people -> persons == NULL) {
    //    fprintf(stderr, "Error: Failed to initialise people.\n");
   //     return false;
  //  }

 //   return true;
//}

int main(void) {
    FreeList freeList;
    initFreeList(&freeList);

    //Person adi = {"Adi Memedovski", 19};
    //Person mysha = {"Mysha Rahim", 18};
    
    //People people;
    //initPeople(&freeList, &people);
    
    //people.persons[0] = adi;
    //people.persons[1] = mysha;
}
