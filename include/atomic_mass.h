#pragma once

namespace mendeleev {

// Atomic mass (amu) for a given ZAID. Throws std::runtime_error if not found.
double atomic_mass(int zaid);
} // namespace mendeleev
