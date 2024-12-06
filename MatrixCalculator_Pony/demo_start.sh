sh -c "$(curl --proto '=https' --tlsv1.2 -sSf https://raw.githubusercontent.com/ponylang/ponyup/latest-release/ponyup-init.sh)"
export PATH=/home/mids/m257050/.local/share/ponyup/bin:$PATH
ponyup update ponyc release
code Phase2.pony
ponyc
clear
echo "./demo_sequential.sh"