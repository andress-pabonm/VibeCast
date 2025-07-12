const getElement = (parent, selector) => parent.querySelector(selector);
const getElements = (parent, selector) => parent.querySelectorAll(selector);

const formatTime = (seconds) => {
  const minutes = Math.floor(seconds / 60);
  const remainingSeconds = Math.floor(seconds % 60);
  return `${minutes}:${remainingSeconds < 10 ? "0" : ""}${remainingSeconds}`;
};
