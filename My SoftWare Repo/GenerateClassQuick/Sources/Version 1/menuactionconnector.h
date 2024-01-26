#ifndef MENUACTIONCONNECTOR_H
#define MENUACTIONCONNECTOR_H

#include <QAction>
#include <QList>
#include <QObject>


template<typename Window>
class MenuActionConnector
{
public:
    MenuActionConnector() = default;
    using VVF = void(Window::*)();
    static bool connectAll(Window* w ,QList<QAction*> acts, QList<VVF>fs)
    {
        if(acts.size() != fs.size() || w == nullptr)
        {
            qDebug() << "Don not match size:> menuConnector";
            return false;
        }

        int i = 0;
        for(QAction*& act : acts)
        {
            w->connect(act, &QAction::triggered, w, fs[i]);
            i++;
        }
        return true;
    }
};

template<typename Window>
class CreateMenuActionConnector : public MenuActionConnector<Window>
{};

template<typename Window>
class ViewMenuActionConnector : public MenuActionConnector<Window>
{};

template<typename Window>
class ExportMenuActionConnector : public MenuActionConnector<Window>
{};

template<typename Window>
class ExportSettingMenuActionConnector : public ExportMenuActionConnector<Window>
{};

template<typename Window>
class ExportBehaviourMenuActionConnector : public ExportMenuActionConnector<Window>
{};


template<typename Window>
class StyleSwitchMenuActionConnector : public MenuActionConnector<Window>
{};


#endif // MENUACTIONCONNECTOR_H
