..\xasm tst6811
if errorlevel 1 goto ex
diff tst6811.hex tst6811.sav
if errorlevel 1 goto ex
diff 6811.lst 6811.sav
:ex
