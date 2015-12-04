..\xasm tst8048
if errorlevel 1 goto ex
diff tst8048.hex tst8048.sav
if errorlevel 1 goto ex
diff 8048.lst 8048.sav
:ex
