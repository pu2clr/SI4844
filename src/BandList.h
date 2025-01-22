/**
 * @mainpage SI48XX Custom Band implementation
 * @brief SI4844 Custom Band for SI4844 ARDUINO LIBRARY  
 * @details  This class implements the management of the custom band list in case the receiver developer wants to use 
 * @details  frequency ranges different from those provided by the SI48XX. 
 * @details  This list will start empty and can be referenced and maintained (with the addition and removal of bands) 
 * @details  in the main program using the addCustomBand and removeCustomBand methods implemented in the Si4844 library.
 * @details  These methods are particularly useful for receivers with mechanical band selection switches, where it is 
 * @details  impossible to determine in advance which band will be selected during use. It is crucial for the designer
 * @details  to correctly configure the custom bands so that when the band switch is positioned and the band index is 
 * @details  detected, it matches an entry in the list. If the detected band index is not found in the list, the 
 * @details  receiver will work with default settings.  
 * @details  **Important** Currently, this structure only works for the band selection mode using a mechanical method (Slide Switch or Rotary Band Switch via a resistor network). For custom bands to function correctly, the LNA_EN pin (pin 1 of the Si4844) must be left floating (open). If this pin is pulled up, the custom parameters will be ignored, and the internally defined device parameters will be used.
 * @todo     Future Changes: Unify the band customization methods for both modes. 
*/

#ifndef _BandList_H // Prevent this file from being compiled more than once
#define _BandList_H

struct BandNode {
  int8_t bandIdx;               // Band index
  uint32_t bottomFrequency;     // Lower frequency limit
  uint32_t topFrequency;        // Upper frequency limit
  uint8_t space;                // Band spacing
  BandNode* next;               // Pointer to the next node in the list
};

class BandList {
  private:
    BandNode* head;             // Head of the linked list

  public:
    BandList() : head(nullptr) {}

    // Add a new custom band to the list
    void add(int8_t bandIdx, uint32_t bottomFrequency, uint32_t topFrequency, uint8_t space) {
      BandNode* newBandNode = new BandNode{bandIdx, bottomFrequency, topFrequency, space, nullptr};
      if (!head) {
        head = newBandNode; // If the list is empty, set the new node as the head
      } else {
        BandNode* current = head;
        while (current->next) {
          current = current->next; // Traverse to the end of the list
        }
        current->next = newBandNode; // Add the new node at the end
      }
    }

    // Remove a custom band identified by its bandIdx
    void remove(int8_t bandIdx) {
      if (!head) return; // If the list is empty, do nothing

      // If the head node is the one to be removed
      if (head->bandIdx == bandIdx) {
        BandNode* temp = head;
        head = head->next; // Move the head to the next node
        delete temp;       // Free the memory of the old head
        return;
      }

      // Traverse the list to find the node to remove
      BandNode* current = head;
      while (current->next && current->next->bandIdx != bandIdx) {
        current = current->next;
      }

      // If the node is found, remove it
      if (current->next) {
        BandNode* temp = current->next;
        current->next = current->next->next; // Skip the node to be removed
        delete temp; // Free the memory of the removed node
      }
    }

    // Find a custom band by its bandIdx and return its pointer
    BandNode* findBand(int8_t bandIdx) {
      BandNode* current = head;
      while (current) {
        if (current->bandIdx == bandIdx) return current; // Return the pointer if found
        current = current->next; // Move to the next node
      }
      return nullptr; // Return nullptr if not found
    }
};

#endif // _BandList_H
