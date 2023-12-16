#.bashrc

function findTelnetPort()
{

    pid=$(pgrep -f "^[^vtg].+$1\s")
    ss -naopt | grep -E "LISTEN.*60000.*$pid" | awk '{print $4}' |tr : " "
}


alias telamflb='autotelnet $(findTelnetPort "cplb.*amf")'
alias telepcuplb='autotelnet $(findTelnetPort "epcuplb")'
