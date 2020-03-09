gcc main.c -o daemon_name.out

to execute:
  <p> sudo ./daemon_name.out </p>

to check:
  ps -ajx
  sudo grep -i daemon_name /var/log/syslog

lockfile:
  cd /var/run
  cat daemon.pid
