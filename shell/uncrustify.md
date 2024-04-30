# Windows上用循环和Linux.cfg
for /R %i IN (*.h *.c) DO echo  D:\gitrepo\uncrustify-0.78.1_f-win64\bin\uncrustify.exe -c D:\gitrepo\uncrustify-0.78.1_f-win64\bin\linux.cfg --no-backup %i

```bash
$ uncrustify -c mystyle.cfg -f somefile.c -o somefile.c.unc
$ uncrustify -c mystyle.cfg -f somefile.c > somefile.c.unc
$ uncrustify -c mystyle.cfg somefile.c
$ uncrustify -c mystyle.cfg --no-backup somefile.c
$ uncrustify -c mystyle.cfg *.c
$ uncrustify -c mystyle.cfg --no-backup *.c
```