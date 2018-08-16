#include <QCoreApplication>
#include <QUrl>
#include <QRemoteObjectRegistryHost>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QUrl urlRegistry("tcp://127.0.0.1:9900");
    QRemoteObjectRegistryHost registryHost(urlRegistry);
    registryHost.setName("SensorsRegistry");

    return a.exec();
}
