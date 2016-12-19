@echo START CIFTEP TEST ...
@echo AUNIT BEST
for %%i in (..\data\cif\*.cif) do ciftep %%i -o test\%%~ni.dat -t
@echo GROW BEST
for %%i in (..\data\cif\*.cif) do ciftep %%i -o test\%%~ni-g.dat -cgrow -t
@echo GROW 100
for %%i in (..\data\cif\*.cif) do ciftep %%i -o test\%%~ni-g-100.dat -cgrow -i100 -t
@echo CELL BEST
for %%i in (..\data\cif\*.cif) do ciftep %%i -o test\%%~ni-c.dat -ccell -t
@echo CELL 010
for %%i in (..\data\cif\*.cif) do ciftep %%i -o test\%%~ni-c-010.dat -ccell -i010 -t
@echo CIFTEP TEST COMPLETE.
