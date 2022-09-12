if exist %1 rmdir %1 /S /Q
xcopy %2 %1 /i /h /-y /f