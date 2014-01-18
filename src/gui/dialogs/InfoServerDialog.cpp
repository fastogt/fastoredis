#include "gui/dialogs/InfoServerDialog.h"

#include <QLabel>
#include <QHBoxLayout>

#include "common/qt/converter_patterns.h"

namespace
{
    const QString redisTextServerTemplate = QObject::tr("<h2>Server:</h2><br/>"
                                                  "Redis version: %1<br/>"
                                                  "Redis git_sha1: %2<br/>"
                                                  "Redis git_dirty: %3<br/>"
                                                  "Redis mode: %4<br/>"
                                                  "Os: %5<br/>"
                                                  "Arch: %6<br/>"
                                                  "Multiplexing Api: %7<br/>"
                                                  "Gcc version: %8<br/>"
                                                  "Process id: %9<br/>"
                                                  "Run id: %10<br/>"
                                                  "Tcp port: %11<br/>"
                                                  "Uptime sec: %12<br/>"
                                                  "Uptime days: %13<br/>"
                                                  "Lru clock: %14");

    const QString redisTextClientsTemplate = QObject::tr("<h2>Clients:</h2><br/>"
                                                         "Connected clients_: %1<br/>"
                                                         "Client longest output list: %2<br/>"
                                                         "Client biggest input buf: %3<br/>"
                                                         "Blocked clients: %4");

    const QString redisTextMemoryTemplate = QObject::tr("<h2>Memory:</h2><br/>"
                                                  "Used memory: %1<br/>"
                                                  "Used memory human: %2<br/>"
                                                  "Used memory rss: %3<br/>"
                                                  "Used memory peak: %4<br/>"
                                                  "Used memory peak human: %5<br/>"
                                                  "Used memory lua: %6<br/>"
                                                  "Mem fragmentation ratio: %7<br/>"
                                                  "Mem allocator: %8");

    const QString redisTextPersistenceTemplate = QObject::tr("<h2>Persistence:</h2><br/>"
                                                  "Loading: %1<br/>"
                                                  "Rdb changes since last save: %2<br/>"
                                                  "Rdb bgsave in_progress: %3<br/>"
                                                  "Rdb last save_time: %4<br/>"
                                                  "Rdb last bgsave_status: %5<br/>"
                                                  "Rdb last bgsave time sec: %6<br/>"
                                                  "Rdb current bgsave time sec: %7<br/>"
                                                  "Aof enabled: %8<br/>"
                                                  "Aof rewrite in progress: %9<br/>"
                                                  "Aof rewrite scheduled: %10<br/>"
                                                  "Aof last rewrite time sec: %11<br/>"
                                                  "Aof current rewrite time sec: %12<br/>"
                                                  "Aof last bgrewrite status: %13");

    const QString redisTextStatsTemplate = QObject::tr("<h2>Stats:</h2><br/>"
                                                  "Total connections received: %1<br/>"
                                                  "Total commands processed: %2<br/>"
                                                  "Instantaneous ops per sec: %3<br/>"
                                                  "Rejected connections: %4<br/>"
                                                  "Expired keys: %5<br/>"
                                                  "Evicted keys: %6<br/>"
                                                  "Keyspace hits: %7<br/>"
                                                  "Keyspace misses: %8<br/>"
                                                  "Pubsub channels: %9<br/>"
                                                  "Pubsub patterns: %10<br/>"
                                                  "Latest fork usec: %11");

    const QString redisTextReplicationTemplate = QObject::tr("<h2>Replication:</h2><br/>"
                                                  "Role: %1<br/>"
                                                  "Connected slaves: %2");

    const QString redisTextCpuTemplate = QObject::tr("<h2>Cpu:</h2><br/>"
                                                         "Used cpu sys: %1<br/>"
                                                         "Used cpu user: %2<br/>"
                                                         "Used cpu sys children_: %3<br/>"
                                                         "Used cpu user children_: %4");
}

namespace fastoredis
{
    InfoServerDialog::InfoServerDialog(connectionTypes type, QWidget *parent)
        : QDialog(parent), type_(type)
    {
        serverTextInfo_ = new QLabel;
        hardwareTextInfo_ = new QLabel;
        QHBoxLayout *mainL = new QHBoxLayout(this);
        mainL->addWidget(serverTextInfo_);
        mainL->addWidget(hardwareTextInfo_);
        setLayout(mainL);
        updateText(ServerInfo());
    }

    void InfoServerDialog::startServerInfo(const EventsInfo::ServerInfoRequest &req)
    {

    }

    void InfoServerDialog::finishServerInfo(const EventsInfo::ServerInfoResponce &res)
    {        
        error::ErrorInfo er = res.errorInfo();
        if(!er.isError()){
            if(type_ == REDIS){
                updateText(res.info_);
            }
        }
    }

    void InfoServerDialog::updateText(const ServerInfo &serv)
    {
        using namespace common;
        ServerInfo::Server ser = serv.server_;
        QString textServ = redisTextServerTemplate.arg(utils_qt::toQString(ser.redis_version_))
                .arg(utils_qt::toQString(ser.redis_git_sha1_))
                .arg(utils_qt::toQString(ser.redis_git_dirty_))
                .arg(utils_qt::toQString(ser.redis_mode_))
                .arg(utils_qt::toQString(ser.os_))
                .arg(ser.arch_bits_)
                .arg(utils_qt::toQString(ser.multiplexing_api_))
                .arg(utils_qt::toQString(ser.gcc_version_))
                .arg(ser.process_id_)
                .arg(utils_qt::toQString(ser.run_id_))
                .arg(ser.tcp_port_)
                .arg(ser.uptime_in_seconds_)
                .arg(ser.uptime_in_days_)
                .arg(ser.lru_clock_);

        ServerInfo::Clients cl = serv.clients_;
        QString textCl = redisTextClientsTemplate.arg(cl.connected_clients_)
                .arg(cl.client_longest_output_list_)
                .arg(cl.client_biggest_input_buf_)
                .arg(cl.blocked_clients_);

        ServerInfo::Memory mem = serv.memory_;
        QString textMem = redisTextMemoryTemplate.arg(mem.used_memory_)
                .arg(utils_qt::toQString(mem.used_memory_human_))
                .arg(mem.used_memory_rss_)
                .arg(mem.used_memory_peak_)
                .arg(utils_qt::toQString(mem.used_memory_peak_human_))
                .arg(mem.used_memory_lua_)
                .arg(mem.mem_fragmentation_ratio_)
                .arg(utils_qt::toQString(mem.mem_allocator_));

        ServerInfo::Persistence per = serv.persistence_;
        QString textPer = redisTextPersistenceTemplate.arg(per.loading_)
                .arg(per.rdb_changes_since_last_save_)
                .arg(per.rdb_bgsave_in_progress_)
                .arg(per.rdb_last_save_time_)
                .arg(utils_qt::toQString(per.rdb_last_bgsave_status_))
                .arg(per.rdb_last_bgsave_time_sec_)
                .arg(per.rdb_current_bgsave_time_sec_)
                .arg(per.aof_enabled_)
                .arg(per.aof_rewrite_in_progress_)
                .arg(per.aof_rewrite_scheduled_)
                .arg(per.aof_last_rewrite_time_sec_)
                .arg(per.aof_current_rewrite_time_sec_)
                .arg(utils_qt::toQString(per.aof_last_bgrewrite_status_));

        ServerInfo::Stats stat = serv.stats_;
        QString textStat = redisTextStatsTemplate.arg(stat.total_connections_received_)
                .arg(stat.total_commands_processed_)
                .arg(stat.instantaneous_ops_per_sec_)
                .arg(stat.rejected_connections_)
                .arg(stat.expired_keys_)
                .arg(stat.evicted_keys_)
                .arg(stat.keyspace_hits_)
                .arg(stat.keyspace_misses_)
                .arg(stat.pubsub_channels_)
                .arg(stat.pubsub_patterns_)
                .arg(stat.latest_fork_usec_);

        ServerInfo::Replication repl = serv.replication_;
        QString textRepl = redisTextReplicationTemplate.arg(utils_qt::toQString(repl.role_))
                .arg(repl.connected_slaves_);

        ServerInfo::Cpu cpu = serv.cpu_;
        QString textCpu = redisTextCpuTemplate.arg(cpu.used_cpu_sys_)
                .arg(cpu.used_cpu_user_)
                .arg(cpu.used_cpu_sys_children_)
                .arg(cpu.used_cpu_user_children_);

        serverTextInfo_->setText(textServ + textMem + textCpu);
        hardwareTextInfo_->setText(textCl + textPer + textStat + textRepl);
    }

    void InfoServerDialog::showEvent(QShowEvent *e)
    {
        QDialog::showEvent(e);
        emit showed();
    }
}
