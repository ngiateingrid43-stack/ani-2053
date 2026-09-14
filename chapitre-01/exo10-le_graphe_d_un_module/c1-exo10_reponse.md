# Exercice 10

## Arbre de dependance

````
                                                                         NKCanvas
                                                                            |
                                                                            | depends de 
                        ----------------------------------------------------------------------------------------------------------------------
                        |                 |                 |                         |                 |                 |                  |            
                        |                 |                 |                         |                 |                 |                  | 
                    NKWindow           NKFont           NKImage                    NKStream           NKTime           NKGlad           NKThreading 
                        |                 |                 |                          |                 |                                   | 
                        |                 |                 |                          |                 |                                   |            
                        |depends          |depends          |depends                   |depends          |depends                            | depends
        ----------------------    -------------------   ----------------            --------------       --------------                    ---------------
        NKPlatform NKCore        NKPlatform  NKCore     NKPlatform                 NKCore NKPlatform    NKContainers                        NKCore                     
        NKLogger NKMath          NKMemory  NKMath       NKCore NKMath              NKLogger NKMemory    NKLogger NKCore                     NKContainers NKMemory   
        NKTime NKContainers      NKContainers           NKMemory NKContainers      NKContainers         NKPlatform                          NKPlatform 
        NKThreading NKMemory     NKLogger NKThreading   NKLogger NKThreading       NKFileSystem         NKMemory 
        NKEvent NKFileSystem                            NKFileSystem NKStream      NKThreading                          
````                                
                                 
## Il faut construire 

````
15 projets avant de construire NKCanvas (consernant l'exercie qui nous demande de s'arreter a deux niveau en realite on construit beaucoup plus de projet)
````