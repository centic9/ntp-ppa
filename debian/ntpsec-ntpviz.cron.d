53 *     * * * root if [ ! -d /run/systemd/system ] ; then ntpviz -p 1 -d /var/log/ntpsec -o /var/lib/ntpsec/ntpviz/day  @/etc/ntpviz/options 2> /dev/null ; find /var/lib/ntpsec/ntpviz/day  -type f -mtime +1 -delete ; fi
45 11,23 * * * root if [ ! -d /run/systemd/system ] ; then ntpviz -p 7 -d /var/log/ntpsec -o /var/lib/ntpsec/ntpviz/week @/etc/ntpviz/options 2> /dev/null ; find /var/lib/ntpsec/ntpviz/week -type f -mtime +7 -delete ; fi
*/5 *    * * * root if [ ! -d /run/systemd/system ] && [ -e /run/gpsd.sock ] && [ -x /usr/sbin/ntploggps ] ; then /usr/sbin/ntploggps -o -l /var/log/ntpsec/gpsd 2> /dev/null ; fi
*/5 *    * * * root if [ ! -d /run/systemd/system ] && [ -x /usr/sbin/ntplogtemp ] ; then /usr/sbin/ntplogtemp -o -l /var/log/ntpsec/temps ; fi
