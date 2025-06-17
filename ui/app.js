function enviar() {
  window.enviarMensaje("Hola desde JS").then((res) => {
    document.getElementById("respuesta").innerText = res.status;
  });
}
