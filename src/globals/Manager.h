#pragma once

#include "concerts/ConcertFileSearcher.h"
#include "concerts/ConcertModel.h"
#include "data/Database.h"
#include "media_centers/MediaCenterInterface.h"
#include "movies/MovieModel.h"
#include "movies/file_searcher/MovieFileSearcher.h"
#include "music/MusicFileSearcher.h"
#include "music/MusicModel.h"
#include "scrapers/concert/ConcertScraperInterface.h"
#include "scrapers/image/FanartTv.h"
#include "scrapers/image/ImageProviderInterface.h"
#include "scrapers/movie/MovieScraperInterface.h"
#include "scrapers/music/MusicScraperInterface.h"
#include "scrapers/trailer/TrailerProvider.h"
#include "scrapers/tv_show/TvScraperInterface.h"
#include "settings/Settings.h"
#include "tv_shows/TvShowFileSearcher.h"
#include "tv_shows/TvShowModel.h"
#include "tv_shows/TvShowProxyModel.h"
#include "ui/main/FileScannerDialog.h"
#include "ui/main/MyIconFont.h"
#include "ui/music/MusicFilesWidget.h"
#include "ui/tv_show/TvShowFilesWidget.h"

#include <QString>
#include <QVector>

class MediaCenterInterface;

/**
 * @brief The Manager class
 * This class handles the various interfaces
 */
class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject* parent = nullptr);
    ~Manager() override;

    static Manager* instance();
    QVector<TvScraperInterface*> tvScrapers();
    ELCH_NODISCARD QVector<MediaCenterInterface*> mediaCenters();
    ELCH_NODISCARD QVector<MovieScraperInterface*> movieScrapers();
    ELCH_NODISCARD MovieScraperInterface* scraper(const QString& identifier);
    ELCH_NODISCARD QVector<ConcertScraperInterface*> concertScrapers();
    ELCH_NODISCARD QVector<MusicScraperInterface*> musicScrapers();
    ELCH_NODISCARD QVector<ImageProviderInterface*> imageProviders();
    ELCH_NODISCARD QVector<ImageProviderInterface*> imageProviders(ImageType type);
    ELCH_NODISCARD QVector<TrailerProvider*> trailerProviders();
    ELCH_NODISCARD MediaCenterInterface* mediaCenterInterface();
    ELCH_NODISCARD MediaCenterInterface* mediaCenterInterfaceTvShow();
    ELCH_NODISCARD MediaCenterInterface* mediaCenterInterfaceConcert();
    ELCH_NODISCARD mediaelch::MovieFileSearcher* movieFileSearcher();
    ELCH_NODISCARD TvShowFileSearcher* tvShowFileSearcher();
    ELCH_NODISCARD ConcertFileSearcher* concertFileSearcher();
    ELCH_NODISCARD MusicFileSearcher* musicFileSearcher();
    ELCH_NODISCARD Database* database();
    ELCH_NODISCARD MovieModel* movieModel();
    ELCH_NODISCARD TvShowModel* tvShowModel();
    ELCH_NODISCARD ConcertModel* concertModel();
    ELCH_NODISCARD MusicModel* musicModel();
    ELCH_NODISCARD FileScannerDialog* fileScannerDialog();
    ELCH_NODISCARD FanartTv* fanartTv();
    ELCH_NODISCARD TvShowFilesWidget* tvShowFilesWidget();
    ELCH_NODISCARD MusicFilesWidget* musicFilesWidget();
    ELCH_NODISCARD MyIconFont* iconFont();
    void setTvShowFilesWidget(TvShowFilesWidget* widget);
    void setMusicFilesWidget(MusicFilesWidget* widget);
    void setFileScannerDialog(FileScannerDialog* dialog);
    static QVector<MovieScraperInterface*> constructNativeScrapers(QObject* scraperParent);
    static QVector<MovieScraperInterface*> constructMovieScrapers(QObject* scraperParent);

private:
    QVector<MediaCenterInterface*> m_mediaCenters;
    QVector<MediaCenterInterface*> m_mediaCentersTvShow;
    QVector<MediaCenterInterface*> m_mediaCentersConcert;
    QVector<MovieScraperInterface*> m_movieScrapers;
    QVector<TvScraperInterface*> m_tvScrapers;
    QVector<ConcertScraperInterface*> m_concertScrapers;
    QVector<MusicScraperInterface*> m_musicScrapers;
    QVector<ImageProviderInterface*> m_imageProviders;
    QVector<TrailerProvider*> m_trailerProviders;

    mediaelch::MovieFileSearcher* m_movieFileSearcher = nullptr;
    TvShowFileSearcher* m_tvShowFileSearcher = nullptr;
    ConcertFileSearcher* m_concertFileSearcher = nullptr;
    MovieModel* m_movieModel = nullptr;
    TvShowModel* m_tvShowModel = nullptr;
    ConcertModel* m_concertModel = nullptr;
    MusicModel* m_musicModel = nullptr;
    Database* m_database = nullptr;
    TvShowFilesWidget* m_tvShowFilesWidget = nullptr;
    MusicFilesWidget* m_musicFilesWidget = nullptr;
    FileScannerDialog* m_fileScannerDialog = nullptr;
    MusicFileSearcher* m_musicFileSearcher = nullptr;
    MyIconFont* m_iconFont = nullptr;
};
