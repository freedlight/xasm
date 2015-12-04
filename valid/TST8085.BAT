..\xasm tst8085
if errorlevel 1 goto ex
diff tst8085.hex tst8085.sav
if errorlevel 1 goto ex
diff 8085.lst 8085.sav
:ex
