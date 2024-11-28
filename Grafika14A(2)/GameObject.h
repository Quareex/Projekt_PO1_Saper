#pragma once
#include <iostream>

class GameObject {
public:
    virtual ~GameObject() = default;

    // Metoda czysto wirtualna do inicjalizacji
    virtual void initialize() = 0;

    // Metoda z cia?em (opcjonalna implementacja)
    virtual void destroy() {
        std::cout << "GameObject destroyed.\n";
    }
};


