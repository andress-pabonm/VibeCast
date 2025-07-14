// === Selectores DOM cortos y reutilizables ===

/**
 * Selecciona el primer elemento que coincida con el selector dentro del elemento padre.
 * @param {string} selector - Selector CSS válido.
 * @param {ParentNode} [parent=document] - Elemento padre donde buscar.
 * @param {boolean} [warn=false] - Si se debe mostrar una advertencia si no se encuentra.
 * @returns {Element|null} El primer elemento encontrado o `null`.
 */
const getElement = (selector, parent = document, warn = false) => {
  const element = parent.querySelector(selector);
  if (warn && !element) console.warn(`Elemento no encontrado: ${selector}`);
  return element;
};

/**
 * Selecciona todos los elementos que coincidan con el selector dentro del elemento padre.
 * @param {string} selector - Selector CSS válido.
 * @param {ParentNode} [parent=document] - Elemento padre donde buscar.
 * @param {boolean} [warn=false] - Si se debe mostrar una advertencia si no se encuentran elementos.
 * @returns {NodeListOf<Element>} Lista de elementos encontrados.
 */
const getElements = (selector, parent = document, warn = false) => {
  const elements = parent.querySelectorAll(selector);
  if (warn && elements.length === 0) {
    console.warn(`No se encontraron elementos para el selector: "${selector}"`);
  }
  return elements;
};

// === Utilidades de tiempo ===

/**
 * Convierte segundos en formato mm:ss
 * @param {number} seconds
 * @returns {string} Formato de tiempo
 */
const formatTime = (seconds) => {
  const minutes = Math.floor(seconds / 60);
  const remaining = Math.floor(seconds % 60);
  return `${minutes}:${remaining < 10 ? "0" : ""}${remaining}`;
};

/**
 * Función asíncrona segura con manejo de errores centralizado.
 * @param {Function} fn - Función asíncrona
 * @param {Function} onError - Callback de error
 * @returns {*} Resultado de la función o undefined si hay error
 */
async function safeCall(fn, onError = console.warn) {
  try {
    return await fn();
  } catch (err) {
    onError(err?.message || err);
  }
}

/**
 * Promesa que se resuelve después de un tiempo (delay).
 * @param {number} ms - Milisegundos de espera
 */
const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
