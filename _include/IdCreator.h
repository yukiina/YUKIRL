#ifndef ID_CREATOR_H
#define ID_CREATOR_H
#include <mutex>
#include <iostream>
#include <chrono>
/**
 * Twitter_Snowflake
 * SnowFlake的结构如下(每部分用-分开):
 * 0 - 0000000000 0000000000 0000000000 0000000000 0 - 00000 - 00000 - 000000000000 
 * 保留           时间戳 41位                            10位机器相关       12位序列号
 * SnowFlake的优点是，整体上按照时间自增排序，并且整个分布式系统内不会产生ID碰撞(由数据中心ID和机器ID作区分)
 * 采用常用的 5位 + 5位 表示机器
 */

namespace yukiina{
    class SnowflakeIdWorker{
    public:
        /*构造函数, 传入机器Id, 数据中心Id, 序列*/
        SnowflakeIdWorker(long workerId = 15L, long datacenterId = 15L, long sequence = 0L);
        /*返回下一个Id 生成id*/
        long nextId();
    private:
        /* 使用 C++11, 获取当前时间(毫秒级)*/
        long timeGen();
        /* 当前毫秒序列用完, 阻塞到下一毫秒 */
        long tilNextMillis(long lastTimestamp);
    private:
        /* 开始时间截 (2019-09-14) */
        const long twepoch = 1568476085L;
        /* 机器id所占的位数 */
        const long workerIdBits = 5L;
        /* 数据中心id所占的位数 */
        const long datacenterIdBits = 5L;
        /* 序列在id中占的位数 */
        const long sequenceBits = 12L;

        /* 支持的最大机器id，结果是31 */
        /* -1 ^ (-1 << n) 可以快速算出 n位 表示的最大值*/
        const long maxWorkerId = -1L ^ (-1L << workerIdBits);
        /* 支持的最大数据中心id，结果是31 */
        const long maxDatacenterId = -1L ^ (-1L << datacenterIdBits);
        /* 生成序列的掩码，这里为4095, 防止溢出 */
        const long sequenceMask = -1L ^ (-1L << sequenceBits);

        /* 机器id到末尾的偏移 */
        const long workerIdShift = sequenceBits; // 12
        /* 数据中心id到末尾的偏移 */
        const long datacenterIdShift = sequenceBits + workerIdBits; // 12+5 = 17
        /* 时间戳到末尾的偏移 */
        const long timestampLeftShift = sequenceBits + workerIdBits + datacenterIdBits; // 12+5+5 = 22
    private:
        /* 传入的工作机器ID(0~31) */
        long workerId;
        /** 传入数据中心ID(0~31) */
        long datacenterId;
        /** 当前毫秒内序列(0~4095) */
        long sequence = 0L;
        /** 上次生成ID的时间截 */
        long lastTimestamp = -1L;

        /*互斥锁, 保证线程安全 */
        std::mutex mutex;
    };
}
#endif
