# Exercice 7

## construction de NKCore

### Premiere construction

**Commande de construction**

````
jenga build --target NKCore --config Debug
````

 **Temps mis par la premiere construction**

 ````
════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           22.12s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
````

### Deuxieme construction

**Commande de construction**

````
jenga build --target NKCore --config Debug
````

**Temps mis par la Dexieme construction**

````
════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           0.93s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
````

## Conclusion 
 le temps mis par la premiere construction(***22.12s***) est bien plus grande que celle de la deuxieme construction(***0.93s***) cela s'explique par le fait que le systeme de construction jenga est dote d'un systeme intelligent qui compile uniquement les fichiers qui ont ete modifie ce qui fait que a la deuxieme construction les fichiers sources qui ont ete compiles lors de la premiere construction ne le sont plus lors de la deuxieme construction ce qui explique une difference de temps entre le temps mis de la premiere et la deuxieme construction.