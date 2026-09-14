# Exercice 11

## Resultat de la construction 

````
PS C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath> jenga build --target NKMath --config Debug

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.6.3             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

Loading workspace...

Configuration: Debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (5 projects):
  1. NKPlatform [STATIC_LIB] → 
  2. NKCore [STATIC_LIB] (depends: NKPlatform) → 
  3. NKMemory [STATIC_LIB] (depends: NKCore, NKPlatform) → 
  4. NKContainers [STATIC_LIB] (depends: NKCore, NKMemory, NKPlatform) → 
  5. NKMath [STATIC_LIB] (depends: NKContainers, NKCore, NKMemory, NKPlatform)


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKPlatform                                                       Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 7 source file(s)
✓   [1/7] Compiled: NkArchDetect.cpp
✓   [2/7] Compiled: NkCGXDetect.cpp
✓   [3/7] Compiled: NkCompilerDetect.cpp
✓   [4/7] Compiled: NkEndianness.cpp
✓   [5/7] Compiled: NkEnv.cpp
✓   [6/7] Compiled: NkCPUFeatures.cpp
✓   [7/7] Compiled: NkPlatformConfig.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\NKPlatform.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                            Time: 12.60s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKCore                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 5 source file(s)
✓   [1/5] Compiled: NkLimits.cpp
✓   [2/5] Compiled: NkAssert.cpp
✓   [3/5] Compiled: NkTraits.cpp
✓   [4/5] Compiled: NkBits.cpp
✓   [5/5] Compiled: NkPlatform.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\NKCore.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                            Time: 12.48s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKMemory                                                         Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 14 source file(s)
✓   [1/14] Compiled: NkContainerAllocator.cpp
✓   [2/14] Compiled: NkFunction.cpp
✓   [3/14] Compiled: NkGc.cpp
✓   [4/14] Compiled: NkFunctionSIMD.cpp
✓   [5/14] Compiled: NkGlobalOperators.cpp
✓   [6/14] Compiled: NkHash.cpp
✓   [7/14] Compiled: NkMemory.cpp
✓   [8/14] Compiled: NkMultiLevelAllocator.cpp
✓   [9/14] Compiled: NkPoolAllocator.cpp
✓   [10/14] Compiled: NkProfiler.cpp
✓   [11/14] Compiled: NkTracker.cpp
✓   [12/14] Compiled: NkTag.cpp
✓   [13/14] Compiled: NkUtils.cpp
✓   [14/14] Compiled: NkAllocator.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\NKMemory.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                            Time: 19.05s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKContainers                                                     Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 42 source file(s)
✓   [1/42] Compiled: NkTrie.cpp
✓   [2/42] Compiled: NkUnorderedMap.cpp
✓   [3/42] Compiled: NkMap.cpp
✓   [4/42] Compiled: NkPriorityQueue.cpp
✓   [5/42] Compiled: NkUnorderedSet.cpp
✓   [6/42] Compiled: NkBinaryTree.cpp
✓   [7/42] Compiled: NkSet.cpp
✓   [8/42] Compiled: NkBTree.cpp
✓   [9/42] Compiled: NkHashMap.cpp
✓   [10/42] Compiled: NkArray.cpp
✓   [11/42] Compiled: NkPool.cpp
✓   [12/42] Compiled: NkRingBuffer.cpp
✓   [13/42] Compiled: NkFunction.cpp
✓   [14/42] Compiled: NkBind.cpp
✓   [15/42] Compiled: NkFunctional.cpp
✓   [16/42] Compiled: NkPair.cpp
✓   [17/42] Compiled: NkTuple.cpp
✓   [18/42] Compiled: NkInitializerList.cpp
✓   [19/42] Compiled: NkIterator.cpp
✓   [20/42] Compiled: NkContainers.cpp
✓   [21/42] Compiled: NkDeque.cpp
✓   [22/42] Compiled: NkDoubleList.cpp
✓   [23/42] Compiled: NkList.cpp
✓   [24/42] Compiled: NkVector.cpp
✓   [25/42] Compiled: NkGraph.cpp
✓   [26/42] Compiled: NkQuadTree.cpp
✓   [27/42] Compiled: NkASCII.cpp
✓   [28/42] Compiled: NkEncoding.cpp
✓   [29/42] Compiled: NkUTF16.cpp
✓   [30/42] Compiled: NkUTF32.cpp
✓   [31/42] Compiled: NkUTF8.cpp
✓   [32/42] Compiled: NkBasicString.cpp
✓   [33/42] Compiled: NkFormat.cpp
✓   [34/42] Compiled: NkString.cpp
✓   [35/42] Compiled: NkStringBuilder.cpp
✓   [36/42] Compiled: NkStringHash.cpp
✓   [37/42] Compiled: NkOptional.cpp
✓   [38/42] Compiled: NkStringView.cpp
✓   [39/42] Compiled: NkStringUtils.cpp
✓   [40/42] Compiled: NkResult.cpp
✓   [41/42] Compiled: NkVariant.cpp
✓   [42/42] Compiled: NkSpan.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\NKContainers.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                            Time: 50.89s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: NKMath                                                           Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 12 source file(s)
✓   [1/12] Compiled: NkAngle.cpp
✓   [2/12] Compiled: NkEulerAngle.cpp

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║                                Compilation Error: NkColor.cpp                                ║
╠══════════════════════════════════════════════════════════════════════════════════════════════╣
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src\NKMath\NkColor.cpp:123:9: error: unknown type name 'nkColor'; did you mean 'NkColor'?    ║
║   123 |         nkColor NkColor::FromHSV(const NkHSV& hsv) noexcept                          ║
║       |         ^~~~~~~                                                                      ║
║       |         NkColor                                                                      ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkColor.h:685:19: note: 'NkColor' declared here                                   ║
║   685 |             class NkColor {                                                          ║
║       |                   ^                                                                  ║
║ 1 error generated.                                                                           ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

✗ ✗ Compilation failed: C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\src\NKMath\NkColor.cpp
✓   [4/12] Compiled: NkFunctions.cpp
✓   [5/12] Compiled: NkMat.cpp
✓   [6/12] Compiled: NkQuat.cpp
✓   [7/12] Compiled: NkRandom.cpp
✓   [8/12] Compiled: NkRange.cpp
✓   [9/12] Compiled: NkRectangle.cpp
✓   [10/12] Compiled: NkSIMD.cpp
✓   [11/12] Compiled: NkSegment.cpp
✓   [12/12] Compiled: NkVec.cpp

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✗ Build Failed                                                                Time: 47.96s  │
│ Errors: 2  | Failed files: 1                                                                 │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                  BUILD FAILED                                  
════════════════════════════════════════════════════════════════════════════════
Projects Built:  4/5
Failed:         1
Errors:         2
Time:           2m23.0s
Status:         ✗ FAILURE
════════════════════════════════════════════════════════════════════════════════

Echecs (1) — a corriger :
  ✗ NKMath
````

## Conclusion

Malgres l'erreur les projets suivant ont ete construits sont

```
  1. NKPlatform  
  2. NKCore 
  3. NKMemory 
  4. NKContainers  
```

la construction s'arrete 2m48.7s malgres l'erreure

ce message me fait comprendre peut importe si un fichier du projet visee a une erreur ce dernier ne bloque pas imediatement la construction des projets dont il depends la construction ce poursuit sans construire le fichier erroner du projet ce qui permet de gagner en temps pour moi car il ne suffira que de reconstruire comme le systeme de construction jenga est intelligent il compilera unique ment le fichier nouvellement corriger c'est magique🎇
