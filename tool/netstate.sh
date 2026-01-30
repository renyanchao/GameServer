#!/bin/bash
# monitor.sh

echo "监控服务器连接状态..."
echo "按 Ctrl+C 停止监控"

while true; do
    echo "=== $(date) ==="
    
    # 全连接队列大小 (Recv-Q of LISTEN socket)
    echo "全连接队列 (Recv-Q):"
    ss -lnt | grep :1234
    
    # 半连接队列 (SYN-RECV)
    echo -n "半连接数 (SYN-RECV): "
    ss -ant | grep :1234 | grep SYN-RECV | wc -l
    
    # 已建立连接数
    echo -n "已建立连接 (ESTAB): "
    ss -ant | grep :1234 | grep ESTAB | wc -l
    
    # 服务器进程状态
    echo -n "服务器进程: "
    ps aux | grep ./bin/server | grep -v grep
    
    echo "----------------------------------------"
    sleep 2
done