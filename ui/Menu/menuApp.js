document.addEventListener("DOMContentLoaded", () => {
  window.is_logged_in().then((res) => {
    if (res.message === 0) {
      window.location.replace("../Login/index.html");
    }
  });

  /* AQUI IRIA TODA LA LOGICA PARA CARGAR LOS DATOS DEL SQLITE */
  //   window.canciones().then((res) => {
  //     const recommendationsContainer = document.getElementById(
  //       "recommendations-container"
  //     );
  //     message.forEach((song) => {
  //       const songElement = document.createElement("div");
  //       songElement.className = "song-item";
  //       songElement.innerHTML = `
  //             <div class="song-info">
  //                 <strong>${song.title}</strong>
  //                 <p>${song.artist} · Recomendado por ${song.friend}</p>
  //             </div>
  //             <div class="song-actions">
  //                 <span>${song.duration}</span>
  //                 <i class="fas fa-play"></i>
  //             </div>
  //         `;
  //       recommendationsContainer.appendChild(songElement);
  //     });
  //   });

  /* Aquí se cargarían las recomendaciones */
  //   window.recomendaciones().then((res) => {
  //     const songsContainer = document.getElementById("songs-container");
  //     message.forEach((song) => {
  //       const songElement = document.createElement("div");
  //       songElement.className = "song-item";
  //       songElement.innerHTML = `
  //             <div class="song-info">
  //                 <strong>${song.title}</strong>
  //                 <p>${song.artist}</p>
  //             </div>
  //             <div class="song-actions">
  //                 <span>${song.duration}</span>
  //                 <i class="fas fa-play"></i>
  //                 <i class="fas fa-plus"></i>
  //             </div>
  //         `;
  //       songsContainer.appendChild(songElement);
  //     });
  //   });

  // Simulación de datos
  const mockData = {
    songs: [
      { id: 1, title: "Bohemian Rhapsody", artist: "Queen", duration: "5:55" },
      { id: 2, title: "Imagine", artist: "John Lennon", duration: "3:04" },
      {
        id: 3,
        title: "Billie Jean",
        artist: "Michael Jackson",
        duration: "4:54",
      },
      {
        id: 4,
        title: "Sweet Child O'Mine",
        artist: "Guns N' Roses",
        duration: "5:56",
      },
      { id: 5, title: "AYAYA", artist: "SI", duration: "2:00" },
    ],
    recommendations: [
      {
        id: 1,
        title: "Dancing Queen",
        artist: "ABBA",
        friend: "María García",
        duration: "3:50",
      },
      {
        id: 2,
        title: "Hotel California",
        artist: "Eagles",
        friend: "Carlos López",
        duration: "6:30",
      },
    ],
  };

  // Cargar canciones
  const songsContainer = document.getElementById("songs-container");
  mockData.songs.forEach((song) => {
    const songElement = document.createElement("div");
    songElement.className = "song-item";
    songElement.innerHTML = `
            <div class="song-info">
                <strong>${song.title}</strong>
                <p>${song.artist}</p>
            </div>
            <div class="song-actions">
                <span>${song.duration}</span>
                <i class="fas fa-play"></i>
                <i class="fas fa-plus"></i>
            </div>
        `;
    songsContainer.appendChild(songElement);
  });

  // Cargar recomendaciones
  const recommendationsContainer = document.getElementById(
    "recommendations-container"
  );
  mockData.recommendations.forEach((song) => {
    const songElement = document.createElement("div");
    songElement.className = "song-item";
    songElement.innerHTML = `
            <div class="song-info">
                <strong>${song.title}</strong>
                <p>${song.artist} · Recomendado por ${song.friend}</p>
            </div>
            <div class="song-actions">
                <span>${song.duration}</span>
                <i class="fas fa-play"></i>
            </div>
        `;
    recommendationsContainer.appendChild(songElement);
  });

  // Evento para el botón de cerrar sesión
  // const logoutBtn = document.querySelector(".logout-btn");
  // if (logoutBtn) {
  //   logoutBtn.addEventListener("click", () => {
  //     window.cerrar_sesion().then((res) => {
  //       if (res.message === 1) {
  //         setTimeout(() => {
  //           location.reload();
  //         }, 500);
  //       }
  //     });
  //   });
  // }

  // Eventos para las canciones
  document.querySelectorAll(".song-item").forEach((item) => {
    item.addEventListener("click", (e) => {
      if (
        !e.target.classList.contains("fa-play") &&
        !e.target.classList.contains("fa-plus")
      ) {
        const songTitle = item.querySelector("strong").textContent;
        console.log(`Reproduciendo: ${songTitle}`);
        // En realidad esta forma de incluir la canción a la cola de reproducción está interesante
        // Aquí iría la lógica para reproducir la canción

        // Idealmente así se añadiría la canción a la cola de reproducción
        // window.add_to_queue(id_cancion);
      }
    });
  });

  // Creo que habría que añadirle algo más para saber en qué momento empezar a reproducir las canciones
  // Así como un botón para desplegar el reproductor de canciones
  // Entonces, cuando se pulse ese botón haría algo como:
  // window.next_song().then((res) => {
  //   if (!res.message) {
  //     console.log("No hay más canciones");
  //   } else {
  //     console.log("Reproduciendo siguiente canción.");
  //   }
  // });
  // Y eso sería en bucle cada vez que termine una canción
  // Espero que haya un evento para detectar si se acabó de reproducir un audio.
});
