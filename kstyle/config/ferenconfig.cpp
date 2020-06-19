
/*************************************************************************
 * Copyright (C) 2014 by Hugo Pereira Da Costa <hugo.pereira@free.fr>    *
 *                                                                       *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program; if not, write to the                         *
 * Free Software Foundation, Inc.,                                       *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 *************************************************************************/

#include "ferenconfig.h"

#include "../feren.h"
#include "../config-feren.h"
#include "ferenconfigdata.h"

#include <QDBusMessage>
#include <QDBusConnection>

extern "C"
{
    Q_DECL_EXPORT QWidget* allocate_kstyle_config(QWidget* parent)
    { return new Feren::StyleConfig(parent); }
}

namespace Feren
{

    //__________________________________________________________________
    StyleConfig::StyleConfig(QWidget* parent):
        QWidget(parent)
    {
        setupUi(this);

        // load setup from configData
        load();

        connect( _tabBarDrawCenteredTabs, &QAbstractButton::toggled, this, &StyleConfig::updateChanged );
        connect( _toolBarDrawItemSeparator, &QAbstractButton::toggled, this, &StyleConfig::updateChanged );
        connect( _viewDrawFocusIndicator, &QAbstractButton::toggled, this, &StyleConfig::updateChanged );
        connect( _dockWidgetDrawFrame, &QAbstractButton::toggled, this, &StyleConfig::updateChanged );
        connect( _titleWidgetDrawFrame, &QAbstractButton::toggled, this, &StyleConfig::updateChanged );
        connect( _sidePanelDrawFrame, &QAbstractButton::toggled, this, &StyleConfig::updateChanged );
        connect( _menuItemDrawThinFocus, &QAbstractButton::toggled, this, &StyleConfig::updateChanged );
        connect( _sliderDrawTickMarks, &QAbstractButton::toggled, this, &StyleConfig::updateChanged );
        connect( _splitterProxyEnabled, &QAbstractButton::toggled, this, &StyleConfig::updateChanged );
        connect( _mnemonicsMode, SIGNAL(currentIndexChanged(int)), SLOT(updateChanged()) );
        connect( _animationsEnabled, &QAbstractButton::toggled, this, &StyleConfig::updateChanged );
        connect( _animationsDuration, SIGNAL(valueChanged(int)), SLOT(updateChanged()) );
        connect( _scrollBarAddLineButtons, SIGNAL(currentIndexChanged(int)), SLOT(updateChanged()) );
        connect( _scrollBarSubLineButtons, SIGNAL(currentIndexChanged(int)), SLOT(updateChanged()) );
        connect( _menuOpacity, &QAbstractSlider::valueChanged, this, &StyleConfig::updateChanged );

    }

    //__________________________________________________________________
    void StyleConfig::save()
    {
        StyleConfigData::setTabBarDrawCenteredTabs( _tabBarDrawCenteredTabs->isChecked() );
        StyleConfigData::setToolBarDrawItemSeparator( _toolBarDrawItemSeparator->isChecked() );
        StyleConfigData::setViewDrawFocusIndicator( _viewDrawFocusIndicator->isChecked() );
        StyleConfigData::setDockWidgetDrawFrame( _dockWidgetDrawFrame->isChecked() );
        StyleConfigData::setTitleWidgetDrawFrame( _titleWidgetDrawFrame->isChecked() );
        StyleConfigData::setSidePanelDrawFrame( _sidePanelDrawFrame->isChecked() );
        StyleConfigData::setMenuItemDrawStrongFocus( !_menuItemDrawThinFocus->isChecked() );
        StyleConfigData::setSliderDrawTickMarks( _sliderDrawTickMarks->isChecked() );
        StyleConfigData::setSplitterProxyEnabled( _splitterProxyEnabled->isChecked() );
        StyleConfigData::setMnemonicsMode( _mnemonicsMode->currentIndex() );
        StyleConfigData::setScrollBarAddLineButtons( _scrollBarAddLineButtons->currentIndex() );
        StyleConfigData::setScrollBarSubLineButtons( _scrollBarSubLineButtons->currentIndex() );
        StyleConfigData::setAnimationsEnabled( _animationsEnabled->isChecked() );
        StyleConfigData::setAnimationsDuration( _animationsDuration->value() );
        StyleConfigData::setMenuOpacity( _menuOpacity->value() );

        StyleConfigData::self()->save();

        // emit dbus signal
        QDBusMessage message( QDBusMessage::createSignal( QStringLiteral( "/FerenStyle" ),  QStringLiteral( "org.kde.Feren.Style" ), QStringLiteral( "reparseConfiguration" ) ) );
        QDBusConnection::sessionBus().send(message);

    }

    //__________________________________________________________________
    void StyleConfig::defaults()
    {
        StyleConfigData::self()->setDefaults();
        load();
    }

    //__________________________________________________________________
    void StyleConfig::reset()
    {
        // reparse configuration
        StyleConfigData::self()->load();

        load();
    }

    //__________________________________________________________________
    void StyleConfig::updateChanged()
    {

        bool modified( false );

        // check if any value was modified
        if( _tabBarDrawCenteredTabs->isChecked() != StyleConfigData::tabBarDrawCenteredTabs() ) modified = true;
        else if( _toolBarDrawItemSeparator->isChecked() != StyleConfigData::toolBarDrawItemSeparator() ) modified = true;
        else if( _viewDrawFocusIndicator->isChecked() != StyleConfigData::viewDrawFocusIndicator() ) modified = true;
        else if( _dockWidgetDrawFrame->isChecked() != StyleConfigData::dockWidgetDrawFrame() ) modified = true;
        else if( _titleWidgetDrawFrame->isChecked() != StyleConfigData::titleWidgetDrawFrame() ) modified = true;
        else if( _sidePanelDrawFrame->isChecked() != StyleConfigData::sidePanelDrawFrame() ) modified = true;
        else if( _menuItemDrawThinFocus->isChecked() == StyleConfigData::menuItemDrawStrongFocus() ) modified = true;
        else if( _sliderDrawTickMarks->isChecked() != StyleConfigData::sliderDrawTickMarks() ) modified = true;
        else if( _mnemonicsMode->currentIndex() != StyleConfigData::mnemonicsMode() ) modified = true;
        else if( _scrollBarAddLineButtons->currentIndex() != StyleConfigData::scrollBarAddLineButtons() ) modified = true;
        else if( _scrollBarSubLineButtons->currentIndex() != StyleConfigData::scrollBarSubLineButtons() ) modified = true;
        else if( _splitterProxyEnabled->isChecked() != StyleConfigData::splitterProxyEnabled() ) modified = true;
        else if( _animationsEnabled->isChecked() != StyleConfigData::animationsEnabled() ) modified = true;
        else if( _animationsDuration->value() != StyleConfigData::animationsDuration() ) modified = true;
        else if( _menuOpacity->value() != StyleConfigData::menuOpacity() ) modified = true;

        emit changed(modified);

    }

    //__________________________________________________________________
    void StyleConfig::load()
    {

        _tabBarDrawCenteredTabs->setChecked( StyleConfigData::tabBarDrawCenteredTabs() );
        _toolBarDrawItemSeparator->setChecked( StyleConfigData::toolBarDrawItemSeparator() );
        _viewDrawFocusIndicator->setChecked( StyleConfigData::viewDrawFocusIndicator() );
        _dockWidgetDrawFrame->setChecked( StyleConfigData::dockWidgetDrawFrame() );
        _titleWidgetDrawFrame->setChecked( StyleConfigData::titleWidgetDrawFrame() );
        _sidePanelDrawFrame->setChecked( StyleConfigData::sidePanelDrawFrame() );
        _menuItemDrawThinFocus->setChecked( !StyleConfigData::menuItemDrawStrongFocus() );
        _sliderDrawTickMarks->setChecked( StyleConfigData::sliderDrawTickMarks() );
        _mnemonicsMode->setCurrentIndex( StyleConfigData::mnemonicsMode() );
        _splitterProxyEnabled->setChecked( StyleConfigData::splitterProxyEnabled() );
        _scrollBarAddLineButtons->setCurrentIndex( StyleConfigData::scrollBarAddLineButtons() );
        _scrollBarSubLineButtons->setCurrentIndex( StyleConfigData::scrollBarSubLineButtons() );
        _animationsEnabled->setChecked( StyleConfigData::animationsEnabled() );
        _animationsDuration->setValue( StyleConfigData::animationsDuration() );
        _menuOpacity->setValue( StyleConfigData::menuOpacity() );

    }

}