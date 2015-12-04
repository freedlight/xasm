..\xasm tst6805
if errorlevel 1 goto ex
diff tst6805.hex tst6805.sav
if errorlevel 1 goto ex
diff 6805.lst 6805.sav
:ex
