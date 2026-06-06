#pragma once

// simple data holder for ammunition quantities found in SupplyBoxes.
class Ammo {
private:
  int quantity;
  float weight;
  float value;
  char name[32];

public:
  Ammo(int q, float w, float v, const char *n)
      : quantity(q), weight(w), value(v) {
    int i = 0;
    while (n[i] != '\0' && i < 31) {
      name[i] = n[i];
      i++;
    }
    name[i] = '\0';
  }

  int getQuantity() const { return quantity; }
  const char *getName() const { return name; }
};