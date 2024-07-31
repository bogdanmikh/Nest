/*struct RendererI {
    virtual ~RendererI() = 0;
    virtual RendererType::Enum getRendererType() const = 0;
    virtual const char *getRendererName() const = 0;
    virtual bool isDeviceRemoved() = 0;
    virtual void flip() = 0;
    virtual void
    createIndexBuffer(IndexBufferHandle handle, const Memory *_mem, uint16_t _flags) = 0;
    virtual void destroyIndexBuffer(IndexBufferHandle handle) = 0;
    virtual void createVertexLayout(VertexLayoutHandle handle, const VertexLayout &_layout) = 0;
    virtual void destroyVertexLayout(VertexLayoutHandle handle) = 0;
    virtual void createVertexBuffer(
        VertexBufferHandle handle,
        const Memory *_mem,
        VertexLayoutHandle _layoutHandle,
        uint16_t _flags
    ) = 0;
    virtual void destroyVertexBuffer(VertexBufferHandle handle) = 0;
    virtual void
    createDynamicIndexBuffer(IndexBufferHandle handle, uint32_t _size, uint16_t _flags) = 0;
    virtual void updateDynamicIndexBuffer(
        IndexBufferHandle handle, uint32_t _offset, uint32_t _size, const Memory *_mem
    ) = 0;
    virtual void destroyDynamicIndexBuffer(IndexBufferHandle handle) = 0;
    virtual void
    createDynamicVertexBuffer(VertexBufferHandle handle, uint32_t _size, uint16_t _flags) = 0;
    virtual void updateDynamicVertexBuffer(
        VertexBufferHandle handle, uint32_t _offset, uint32_t _size, const Memory *_mem
    ) = 0;
    virtual void destroyDynamicVertexBuffer(VertexBufferHandle handle) = 0;
    virtual void createShader(ShaderHandle handle, const Memory *_mem) = 0;
    virtual void destroyShader(ShaderHandle handle) = 0;
    virtual void createProgram(ProgramHandle handle, ShaderHandle _vsh, ShaderHandle _fsh) = 0;
    virtual void destroyProgram(ProgramHandle handle) = 0;
    virtual void *
    createTexture(TextureHandle handle, const Memory *_mem, uint64_t _flags, uint8_t _skip) = 0;
    virtual void updateTextureBegin(TextureHandle handle, uint8_t _side, uint8_t _mip) = 0;
    virtual void updateTexture(
        TextureHandle handle,
        uint8_t _side,
        uint8_t _mip,
        const Rect &_rect,
        uint16_t _z,
        uint16_t _depth,
        uint16_t _pitch,
        const Memory *_mem
    ) = 0;
    virtual void updateTextureEnd() = 0;
    virtual void readTexture(TextureHandle handle, void *_data, uint8_t _mip) = 0;
    virtual void resizeTexture(
        TextureHandle handle,
        uint16_t _width,
        uint16_t _height,
        uint8_t _numMips,
        uint16_t _numLayers
    ) = 0;
    virtual void overrideInternal(TextureHandle handle, uintptr_t _ptr) = 0;
    virtual uintptr_t getInternal(TextureHandle handle) = 0;
    virtual void destroyTexture(TextureHandle handle) = 0;
    virtual void
    createFrameBuffer(FrameBufferHandle handle, uint8_t _num, const Attachment *_attachment) = 0;
    virtual void createFrameBuffer(
        FrameBufferHandle handle,
        void *_nwh,
        uint32_t _width,
        uint32_t _height,
        TextureFormat::Enum _format,
        TextureFormat::Enum _depthFormat
    ) = 0;
    virtual void destroyFrameBuffer(FrameBufferHandle handle) = 0;
    virtual void createUniform(
        UniformHandle handle, UniformType::Enum _type, uint16_t _num, const char *_name
    ) = 0;
    virtual void destroyUniform(UniformHandle handle) = 0;
    virtual void requestScreenShot(FrameBufferHandle handle, const char *_filePath) = 0;
    virtual void updateViewName(ViewId _id, const char *_name) = 0;
    virtual void updateUniform(uint16_t _loc, const void *_data, uint32_t _size) = 0;
    virtual void invalidateOcclusionQuery(OcclusionQueryHandle handle) = 0;
    virtual void setMarker(const char *_marker, uint16_t _len) = 0;
    virtual void setName(Handle handle, const char *_name, uint16_t _len) = 0;
    virtual void
    submit(Frame *_render, ClearQuad &_clearQuad, TextVideoMemBlitter &_textVideoMemBlitter) = 0;
    virtual void blitSetup(TextVideoMemBlitter &_blitter) = 0;
    virtual void blitRender(TextVideoMemBlitter &_blitter, uint32_t _numIndices) = 0;
};
 */