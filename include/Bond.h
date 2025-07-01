
#ifndef BOND_H
#define BOND_H


struct Bond {
    int atom1;
    int atom2;
    int order;
    
    Bond(int a1, int a2, int order) : atom1(a1), atom2(a2), order(order) {}
};

#endif // BOND_H