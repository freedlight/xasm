..\xasm tst6803
if errorlevel 1 goto ex
diff tst6803.hex tst6803.sav
if errorlevel 1 goto ex
diff 6803.lst 6803.sav
:ex
