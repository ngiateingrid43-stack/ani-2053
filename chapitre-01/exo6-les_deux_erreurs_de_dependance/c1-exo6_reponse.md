# Exercice 6

## Sans dependson

````
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Applications\MonEssai\mai ║
║ n.cpp:1:                                                                                     ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkVec.h:35:                                                                       ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:635:13: warning: 'dllimport' attribute ignored on inline function       ║
║ [-Wignored-attributes]                                                                       ║
║   635 |             NKENTSEU_MATH_API_FORCE_INLINE                                           ║
║       |             ^                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:152:48: note: expanded from macro 'NKENTSEU_MATH_API_FORCE_INLINE'    ║
║ 152 |         #define NKENTSEU_MATH_API_FORCE_INLINE NKENTSEU_MATH_API NKENTSEU_FORCE_INLINE ║
║       |                                                ^                                     ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:96:35: note: expanded from macro 'NKENTSEU_MATH_API'                  ║
║    96 |         #define NKENTSEU_MATH_API NKENTSEU_PLATFORM_API_IMPORT                       ║
║       |                                   ^                                                  ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKPlatf ║
║ orm\src/NKPlatform/NkPlatformExport.h:114:57: note: expanded from macro                      ║
║ 'NKENTSEU_PLATFORM_API_IMPORT'                                                               ║
║   114 |         #define NKENTSEU_PLATFORM_API_IMPORT __declspec(dllimport)                   ║
║       |                                                         ^                            ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Applications\MonEssai\mai ║
║ n.cpp:1:                                                                                     ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkVec.h:35:                                                                       ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:653:29: warning: 'nkentseu::math::ToString' redeclared inline;          ║
║ 'dllimport' attribute ignored [-Wignored-attributes]                                         ║
║   653 |             inline NkString ToString(const NkAngleT<float32>& a)                     ║
║       |                             ^                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:658:34: warning: 'nkentseu::math::operator<<' redeclared inline;        ║
║ 'dllimport' attribute ignored [-Wignored-attributes]                                         ║
║ 658 |             inline std::ostream& operator<<(std::ostream& os, const NkAngleT<float32>& ║
║ a)                                                                                           ║
║       |                                  ^                                                   ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:669:29: warning: 'nkentseu::math::ToString' redeclared inline;          ║
║ 'dllimport' attribute ignored [-Wignored-attributes]                                         ║
║   669 |             inline NkString ToString(const NkAngleT<float64>& a)                     ║
║       |                             ^                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:674:34: warning: 'nkentseu::math::operator<<' redeclared inline;        ║
║ 'dllimport' attribute ignored [-Wignored-attributes]                                         ║
║ 674 |             inline std::ostream& operator<<(std::ostream& os, const NkAngleT<float64>& ║
║ a)                                                                                           ║
║       |                                  ^                                                   ║
║ 228 warnings generated.                                                                      ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝
ℹ Linking...
╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║                                Compilation Error: Link Failed                                ║
╠══════════════════════════════════════════════════════════════════════════════════════════════╣
║ C:/msys64/ucrt64/bin/ld: cannot find -lNKMath: No such file or directory                     ║
║ C:/msys64/ucrt64/bin/ld: cannot find -lNKContainers: No such file or directory               ║
║ C:/msys64/ucrt64/bin/ld: cannot find -lNKMemory: No such file or directory                   ║
║ C:/msys64/ucrt64/bin/ld: cannot find -lNKCore: No such file or directory                     ║
║ C:/msys64/ucrt64/bin/ld: cannot find -lNKPlatform: No such file or directory                 ║
║ clang++: error: linker command failed with exit code 1 (use -v to see invocation)            ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝
✗ Link failed: Build\Bin\Debug-Windows\MonEssai.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✗ Build Failed                                                               Time: 1m14.9s  │
│ Errors: 1  | Warnings: 228  | Failed files: 1                                                │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                  BUILD FAILED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  0/1
Failed:         1
Errors:         1
Warnings:       228
Time:           1m15.0s
Status:         ✗ FAILURE
════════════════════════════════════════════════════════════════════════════════

Echecs (1) — a corriger :
  ✗ MonEssai

````

## Sans Links

````
║       |                                                         ^                            ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Applications\MonEssai\mai ║
║ n.cpp:1:                                                                                     ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkVec.h:35:                                                                       ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:465:21: warning: 'dllimport' attribute ignored on inline function       ║
║ [-Wignored-attributes]                                                                       ║
║   465 |                     NKENTSEU_MATH_API_FORCE_INLINE                                   ║
║       |                     ^                                                                ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:152:48: note: expanded from macro 'NKENTSEU_MATH_API_FORCE_INLINE'    ║
║ 152 |         #define NKENTSEU_MATH_API_FORCE_INLINE NKENTSEU_MATH_API NKENTSEU_FORCE_INLINE ║
║       |                                                ^                                     ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:96:35: note: expanded from macro 'NKENTSEU_MATH_API'                  ║
║    96 |         #define NKENTSEU_MATH_API NKENTSEU_PLATFORM_API_IMPORT                       ║
║       |                                   ^                                                  ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKPlatf ║
║ orm\src/NKPlatform/NkPlatformExport.h:114:57: note: expanded from macro                      ║
║ 'NKENTSEU_PLATFORM_API_IMPORT'                                                               ║
║   114 |         #define NKENTSEU_PLATFORM_API_IMPORT __declspec(dllimport)                   ║
║       |                                                         ^                            ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Applications\MonEssai\mai ║
║ n.cpp:1:                                                                                     ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkVec.h:35:                                                                       ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:505:28: warning: 'dllimport' attribute ignored on inline function       ║
║ [-Wignored-attributes]                                                                       ║
║   505 |                     static NKENTSEU_MATH_API_FORCE_INLINE                            ║
║       |                            ^                                                         ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:152:48: note: expanded from macro 'NKENTSEU_MATH_API_FORCE_INLINE'    ║
║ 152 |         #define NKENTSEU_MATH_API_FORCE_INLINE NKENTSEU_MATH_API NKENTSEU_FORCE_INLINE ║
║       |                                                ^                                     ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:96:35: note: expanded from macro 'NKENTSEU_MATH_API'                  ║
║    96 |         #define NKENTSEU_MATH_API NKENTSEU_PLATFORM_API_IMPORT                       ║
║       |                                   ^                                                  ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKPlatf ║
║ orm\src/NKPlatform/NkPlatformExport.h:114:57: note: expanded from macro                      ║
║ 'NKENTSEU_PLATFORM_API_IMPORT'                                                               ║
║   114 |         #define NKENTSEU_PLATFORM_API_IMPORT __declspec(dllimport)                   ║
║       |                                                         ^                            ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Applications\MonEssai\mai ║
║ n.cpp:1:                                                                                     ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkVec.h:35:                                                                       ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:559:13: warning: 'dllimport' attribute ignored on inline function       ║
║ [-Wignored-attributes]                                                                       ║
║   559 |             NKENTSEU_MATH_API_FORCE_INLINE                                           ║
║       |             ^                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:152:48: note: expanded from macro 'NKENTSEU_MATH_API_FORCE_INLINE'    ║
║ 152 |         #define NKENTSEU_MATH_API_FORCE_INLINE NKENTSEU_MATH_API NKENTSEU_FORCE_INLINE ║
║       |                                                ^                                     ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:96:35: note: expanded from macro 'NKENTSEU_MATH_API'                  ║
║    96 |         #define NKENTSEU_MATH_API NKENTSEU_PLATFORM_API_IMPORT                       ║
║       |                                   ^                                                  ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKPlatf ║
║ orm\src/NKPlatform/NkPlatformExport.h:114:57: note: expanded from macro                      ║
║ 'NKENTSEU_PLATFORM_API_IMPORT'                                                               ║
║   114 |         #define NKENTSEU_PLATFORM_API_IMPORT __declspec(dllimport)                   ║
║       |                                                         ^                            ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Applications\MonEssai\mai ║
║ n.cpp:1:                                                                                     ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkVec.h:35:                                                                       ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:589:13: warning: 'dllimport' attribute ignored on inline function       ║
║ [-Wignored-attributes]                                                                       ║
║   589 |             NKENTSEU_MATH_API_FORCE_INLINE                                           ║
║       |             ^                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:152:48: note: expanded from macro 'NKENTSEU_MATH_API_FORCE_INLINE'    ║
║ 152 |         #define NKENTSEU_MATH_API_FORCE_INLINE NKENTSEU_MATH_API NKENTSEU_FORCE_INLINE ║
║       |                                                ^                                     ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:96:35: note: expanded from macro 'NKENTSEU_MATH_API'                  ║
║    96 |         #define NKENTSEU_MATH_API NKENTSEU_PLATFORM_API_IMPORT                       ║
║       |                                   ^                                                  ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKPlatf ║
║ orm\src/NKPlatform/NkPlatformExport.h:114:57: note: expanded from macro                      ║
║ 'NKENTSEU_PLATFORM_API_IMPORT'                                                               ║
║   114 |         #define NKENTSEU_PLATFORM_API_IMPORT __declspec(dllimport)                   ║
║       |                                                         ^                            ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Applications\MonEssai\mai ║
║ n.cpp:1:                                                                                     ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkVec.h:35:                                                                       ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:617:13: warning: 'dllimport' attribute ignored on inline function       ║
║ [-Wignored-attributes]                                                                       ║
║   617 |             NKENTSEU_MATH_API_FORCE_INLINE                                           ║
║       |             ^                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:152:48: note: expanded from macro 'NKENTSEU_MATH_API_FORCE_INLINE'    ║
║ 152 |         #define NKENTSEU_MATH_API_FORCE_INLINE NKENTSEU_MATH_API NKENTSEU_FORCE_INLINE ║
║       |                                                ^                                     ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:96:35: note: expanded from macro 'NKENTSEU_MATH_API'                  ║
║    96 |         #define NKENTSEU_MATH_API NKENTSEU_PLATFORM_API_IMPORT                       ║
║       |                                   ^                                                  ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKPlatf ║
║ orm\src/NKPlatform/NkPlatformExport.h:114:57: note: expanded from macro                      ║
║ 'NKENTSEU_PLATFORM_API_IMPORT'                                                               ║
║   114 |         #define NKENTSEU_PLATFORM_API_IMPORT __declspec(dllimport)                   ║
║       |                                                         ^                            ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Applications\MonEssai\mai ║
║ n.cpp:1:                                                                                     ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkVec.h:35:                                                                       ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:635:13: warning: 'dllimport' attribute ignored on inline function       ║
║ [-Wignored-attributes]                                                                       ║
║   635 |             NKENTSEU_MATH_API_FORCE_INLINE                                           ║
║       |             ^                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:152:48: note: expanded from macro 'NKENTSEU_MATH_API_FORCE_INLINE'    ║
║ 152 |         #define NKENTSEU_MATH_API_FORCE_INLINE NKENTSEU_MATH_API NKENTSEU_FORCE_INLINE ║
║       |                                                ^                                     ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkMathApi.h:96:35: note: expanded from macro 'NKENTSEU_MATH_API'                  ║
║    96 |         #define NKENTSEU_MATH_API NKENTSEU_PLATFORM_API_IMPORT                       ║
║       |                                   ^                                                  ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKPlatf ║
║ orm\src/NKPlatform/NkPlatformExport.h:114:57: note: expanded from macro                      ║
║ 'NKENTSEU_PLATFORM_API_IMPORT'                                                               ║
║   114 |         #define NKENTSEU_PLATFORM_API_IMPORT __declspec(dllimport)                   ║
║       |                                                         ^                            ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Applications\MonEssai\mai ║
║ n.cpp:1:                                                                                     ║
║ In file included from                                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkVec.h:35:                                                                       ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:653:29: warning: 'nkentseu::math::ToString' redeclared inline;          ║
║ 'dllimport' attribute ignored [-Wignored-attributes]                                         ║
║   653 |             inline NkString ToString(const NkAngleT<float32>& a)                     ║
║       |                             ^                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:658:34: warning: 'nkentseu::math::operator<<' redeclared inline;        ║
║ 'dllimport' attribute ignored [-Wignored-attributes]                                         ║
║ 658 |             inline std::ostream& operator<<(std::ostream& os, const NkAngleT<float32>& ║
║ a)                                                                                           ║
║       |                                  ^                                                   ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:669:29: warning: 'nkentseu::math::ToString' redeclared inline;          ║
║ 'dllimport' attribute ignored [-Wignored-attributes]                                         ║
║   669 |             inline NkString ToString(const NkAngleT<float64>& a)                     ║
║       |                             ^                                                        ║
║ C:\Users\DELL\Desktop\BUREAU_2\Rihen\Nkentseu-main-1\Nkentseu-main\Kernel\Foundation\NKMath\ ║
║ src/NKMath/NkAngle.h:674:34: warning: 'nkentseu::math::operator<<' redeclared inline;        ║
║ 'dllimport' attribute ignored [-Wignored-attributes]                                         ║
║ 674 |             inline std::ostream& operator<<(std::ostream& os, const NkAngleT<float64>& ║
║ a)                                                                                           ║
║       |                                  ^                                                   ║
║ 228 warnings generated.                                                                      ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝
ℹ Linking...
✓ Built: Build\Bin\Debug-Windows\MonEssai.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                            Time: 23.03s  │
│ Warnings: 228                                                                                │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  6/6
Warnings:       228
Time:           3m23.1s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
````

## Conclusion 

Sans 'limks' le projets se construit avec certain avertissement alors que sans 'dependson' le projet ne se construit pas du tout 