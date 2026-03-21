function applyAll(config)
{
                desktops().
                forEach(d=>
                        {
                                        d.wallpaperPlugin="org.kde.image";
                                        d.currentConfigGroup= ['Wallpaper','org.kde.image','General'];
                                        d.writeConfig('Image',config.wallpaperPath);
                                        d.writeConfig('FillMode',config.fillMode);
                        });
}
function apply(config)
{
                const monitors = config.monitors;
                desktops().
                forEach(d=>
                        {
                                        if(monitors.includes(d.screen)){
                                                        d.wallpaperPlugin="org.kde.image";
                                                        d.currentConfigGroup=['Wallpaper','org.kde.image','General'];
                                                        d.writeConfig('Image',config.wallpaperPath);
                                                        d.writeConfig('FillMode',config.fillMode);
                                        }
                        });
}
