..\xasm tst65c00
if errorlevel 1 goto ex
diff tst65c00.hex tst65c00.sav
if errorlevel 1 goto ex
diff 65c00.lst 65c00.sav
:ex
