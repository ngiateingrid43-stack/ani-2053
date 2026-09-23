# Exerxice 8

## Taille de NKCore.lib construit avec la configuration Release

````
17 ko
````

## Taille de NKCore.lib construit avec la configuration Debug

````
150 ko
````

### Donc
````
la taille du fichier binaire obtenu grace a la construction avec la configuration Release est bien plus petit que celui obtenue grace la construction avec la configuration Debug.
````

## Temps de construction avec Release

````
════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           41.83s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
````

## Temps de construction avec Debug

````
════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Time:           22.12s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
````

## Comparaison des deux temps

La construction avec la configuration Debug est plus petite que celle avec la configuration Release.


## La partie du .jenga qui justifie cest 4 chiffres est 

````
with filter("config:Debug"):
        defines(["_DEBUG", "DEBUG", "NKENTSEU_DEBUG"])
        optimize("Off")
        symbols(True)
    with filter("config:Release"):
        defines(["NDEBUG", "RELEASE", "NKENTSEU_RELEASE"])
        optimize("Speed")
        symbols(False)
````