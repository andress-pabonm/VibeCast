document.addEventListener('DOMContentLoaded', () => {
    // Simula la carga de playlists
    const playlists = [
        { name: "Playlist 1" },
        { name: "Playlist 2" },
        { name: "Playlist 3" }
    ];

    const playlistsContainer = document.querySelector('.playlists');
    playlists.forEach(playlist => {
        const playlistElement = document.createElement('div');
        playlistElement.textContent = playlist.name;
        playlistsContainer.appendChild(playlistElement);
    });

    // Simula la carga de canciones de la base de datos
    const databaseSongs = [
        { title: "Cancion 1", artist: "Artista 1" },
        { title: "Cancion 2", artist: "Artista 2" },
        { title: "Cancion 3", artist: "Artista 3" }
    ];

    const databaseSongContainer = document.querySelector('.database-songs');
    databaseSongs.forEach(song => {
        const songElement = document.createElement('div');
        songElement.textContent = `${song.title} - ${song.artist}`;
        databaseSongContainer.appendChild(songElement);
    });

    // Simula la carga de amigos
    const friends = [
        { name: "Amigo 1" },
        { name: "Amigo 2" },
        { name: "Amigo 3" }
    ];

    const friendsListContainer = document.querySelector('.friends');
    friends.forEach(friend => {
        const friendElement = document.createElement('div');
        friendElement.textContent = friend.name;
        friendsListContainer.appendChild(friendElement);
    });

    // Simula la carga de canciones de amigos
    const friendSongs = [
        { title: "Cancion Amigo 1", artist: "Artista Amigo 1" },
        { title: "Cancion Amigo 2", artist: "Artista Amigo 2" },
        { title: "Cancion Amigo 3", artist: "Artista Amigo 3" }
    ];

    const friendSongContainer = document.querySelector('.friend-songs');
    friendSongs.forEach(song => {
        const songElement = document.createElement('div');
        songElement.textContent = `${song.title} - ${song.artist}`;
        songElement.className = 'song-item';
        friendSongContainer.appendChild(songElement);
    });
});