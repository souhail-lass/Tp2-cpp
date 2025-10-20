#!/bin/bash

echo "=== TEST DE TOUS LES NIVEAUX ==="
echo ""

# Niveau 1
echo "NIVEAU 1: Max 2 passagers, aucun prêt, pas de contraintes horaires"
./tp2 exemple1.txt
echo ""

# Niveau 2  
echo "NIVEAU 2: Max 2 passagers, prêt possible, pas de contraintes horaires"
sed 's/N/O/g' exemple1.txt > exemple2_test.txt
./tp2 exemple2_test.txt
echo ""

# Niveau 3
echo "NIVEAU 3: Max 2 passagers, prêt possible, avec contraintes horaires"
sed 's/0h00/7h00/g' exemple1.txt | sed 's/N/O/g' > exemple3_test.txt
./tp2 exemple3_test.txt
echo ""

# Niveau 4
echo "NIVEAU 4: Max 3 passagers, prêt possible, avec contraintes horaires"
sed 's/2	O/3	O/g' exemple1.txt | sed 's/0h00/7h00/g' > exemple4_test.txt
./tp2 exemple4_test.txt
echo ""

# Niveau 5
echo "NIVEAU 5: Passagers illimités, prêt possible, avec contraintes horaires"
sed 's/2	O/5	O/g' exemple1.txt | sed 's/0h00/7h00/g' > exemple5_test.txt
./tp2 exemple5_test.txt
echo ""

echo "=== TESTS TERMINÉS ==="
