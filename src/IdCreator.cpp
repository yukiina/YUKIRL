#include "IdCreator.h"

using namespace yukiina;

/*构造函数, 传入机器Id, 数据中心Id, 序列*/
SnowflakeIdWorker::SnowflakeIdWorker(long workerId, long datacenterId, long sequence) {
    // 检查传参合法性 0~31
    if (workerId > maxWorkerId || workerId < 0) {
        std::cerr << "workerId输入不合法" << std::endl;
        exit(1);
    }
    if (datacenterId > maxDatacenterId || datacenterId < 0) {
        std::cerr << "datacenterId输入不合法" << std::endl;
        exit(1);
    }
    this->workerId = workerId;
    this->datacenterId = datacenterId;
    this->sequence = sequence;
}

/* 生成一个id, 该方法必须保证线程安全*/
/* 使用互斥锁 + 原子操作 */
long SnowflakeIdWorker::nextId() {
    /* 保证线程安全 */
    std::unique_lock<std::mutex> lock{ mutex };
    long timestamp = 0L;
    timestamp = timeGen();

    /* 如果这次的时间戳小于最后一次生成的时间戳, 说明发生了系统回退的异常*/
    if (timestamp < lastTimestamp) {
        std::cerr << "系统时钟可能发生回退" << lastTimestamp - timestamp << " 毫秒";
        exit(1);
    }

    if (lastTimestamp == timestamp) {
        /* 如果是同一毫秒内需要生成id, 需要进行序列递增*/
        sequence = (sequence + 1) & sequenceMask;
        if (0 == sequence) {
            /* 毫秒内序列溢出(大于 4095), 阻塞到下一个毫秒,获得新的时间戳 */
            timestamp = tilNextMillis(lastTimestamp);
        } 

    } else {
        sequence = 0;
    }

    lastTimestamp = timestamp;
    
    /*通过移位操作和位或操作, 设置每一位然后返回 id*/
    return ((timestamp - twepoch) << timestampLeftShift)
                | (datacenterId << datacenterIdShift)
                | (workerId << workerIdShift)
                | sequence;
}

/* 当前毫秒已经生成了 4095个id, 阻塞到下一毫秒 */
long SnowflakeIdWorker::tilNextMillis(long lastTimestamp) {
    long timestamp = timeGen(); 
    /* 一直阻塞到下一毫秒, 返回下一毫秒的时间戳 */
    while(timestamp <= lastTimestamp) {
        timestamp = timeGen();
    }
    return timestamp;
}

/* 使用 C++11, 获取当前时间(毫秒级)*/
long SnowflakeIdWorker::timeGen() {
    /* C++11 高精度时间(精确到毫秒), 返回当前时间戳*/
    auto t = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
    return t.time_since_epoch().count();
}