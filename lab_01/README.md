gcc main.c -o daemon_name.out

to execute:

  sudo ./daemon_name.out

to check:
  ps -ajx
  sudo grep -i daemon_name /var/log/syslog

lockfile:
  cd /var/run
  cat daemon.pid
