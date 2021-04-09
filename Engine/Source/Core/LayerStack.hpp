#pragma once

#include "Core/Aliases.hpp"
#include "Core/Layer.hpp"

namespace QMBT
{
	/**
	 * @brief Hold all the layers in use by the application. Layers are divided into two sections:
	 * Normal layers and Overlays. Overlays will always appear on top of any normal layer.
	 */
	class LayerStack
	{
	  public:
		LayerStack();
		~LayerStack();

		/**
		 * @brief Push a layer to the stack. 
		 * @details The layer will be rendered on 
		 * top of any previously pushed layers
		 * @param layer The layer to be pushed
		 */
		void PushLayer(Layer* layer);
		/**
		 * @brief Push an overlay to the stack. 
		 * @details The overlay will be rendered on top of all normal layers
		 * and on top of any previously pushed overlays
		 * @param overlay The overlay to be pushed
		 */
		void PushOverlay(Layer* overlay);
		/**
		 * @brief Pop a layer from the stack
		 * 
		 * @param layer The layer to be popped
		 */
		void PopLayer(Layer* layer);
		/**
		 * @brief Pop an overlay from the stack. 
		 * 
		 * @param overlay The overlay to be popped
		 */
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

		//const std::vector<Layer*>::iterator cbegin() { return m_Layers.cbegin(); }
		//const std::vector<Layer*>::iterator cend() { return m_Layers.cend(); }

	  private:
		//Stores all the layers and overlays
		std::vector<Layer*> m_Layers;
		/*
		Acts as the barrier between layers and overlays. Layers are stored in the first half
		before the insert), while overlays are stored after the insert.
		New layers are added at the current insert position while overlays are added at the end 
		of the list.
		*/
		unsigned int m_LayerInsertIndex = 0;
	};
} // namespace QMBT